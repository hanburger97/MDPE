/*
   Copyright 2015 Bloomberg Finance L.P.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

/**
 * Access control for remote access
 *
 */

#include <plhash.h>
#include "list.h"

#include "comdb2.h"
#include "sql.h"
#include "sqlite3.h"
#include "sqliteInt.h"
#include "vdbeInt.h"
#include "fdb_fend.h"
#include "fdb_boots.h"
#include "fdb_comm.h"
#include "fdb_util.h"
#include "fdb_fend_cache.h"
#include "fdb_access.h"
#include "logmsg.h"

/**
 * this is per sqlclntstate structure
 * until we have some parallelism involved
 * these fields are unprotected and accessed
 * serially
 *
 */
typedef struct fdb_access_elem {
    int type;       /* type of access: RWX */
    char *fullname; /* [dbname.tblname|dbname] */
    char *dbname;   /* strdup(dbname) for easy lookup */
    char *tblname;  /* pointer iside fqdbname, if any */
    linkc_t lnk;
} fdb_access_elem_t;

struct fdb_access {
    hash_t *wlst_bydbname; /* presence in this list means implies all tables */
    hash_t *wlst_byfullname; /* presence in this list give per table access */
    listc_t lst;
};

static int access_violations;
static int _access_control_add(fdb_access_t *acc, enum fdb_access_type type,
                               char *fullname /*malloced*/);
static int _access_control_clear(fdb_access_t *acc);

/**
 *   Create remote access control for a client
 *
 */
int fdb_access_control_create(struct sqlclntstate *clnt, char *str)
{
    fdb_access_t *acc = clnt->fdb_state.access;
    int rc = 0;
    enum fdb_access_type type = ACCESS_REMOTE_INV;
    int created = 0;
    int added = 0;

    if (!acc) {
        clnt->fdb_state.access = acc =
            (fdb_access_t *)calloc(1, sizeof(fdb_access_t));
        if (!acc) {
            logmsg(LOGMSG_ERROR, "%s: OOM!\n", __func__);
            return -1;
        }

        acc->wlst_bydbname =
            hash_init_strptr(offsetof(fdb_access_elem_t, dbname));
        acc->wlst_byfullname =
            hash_init_strptr(offsetof(fdb_access_elem_t, fullname));
        listc_init(&acc->lst, offsetof(struct fdb_access_elem, lnk));
        created = 1;
    }

    rc = 0;

    if (str) {
        char *tok;
        int ltok, st;
        int lstr = strlen(str);
        char *fullname = NULL;

        st = 0;
        do {
            tok = segtok(str, lstr, &st, &ltok);
            if (ltok == 0 || !tok) {
                if (type == ACCESS_REMOTE_INV) {
                    /* deny anything */
                    if (!created) {
                        /* remote access denied */
                        logmsg(LOGMSG_ERROR, "%s: deny all remote access \"%s\"!\n",
                                __func__, str);
                        rc = _access_control_clear(acc);
                    } else {
                        /* structure remains empty, denial any remote access */
                    }
                } else if (!added) {
                    /* specified type but missing tables? */
                    logmsg(LOGMSG_ERROR, "%s: missing table list in string \"%s\"\n",
                            __func__, str);
                    rc = -1;
                }
                break;
            }

            fullname = tokdup(tok, ltok);

            if (type == ACCESS_REMOTE_INV) {
                if (ltok == 4 && strncasecmp(fullname, "read", 5) == 0) {
                    type = ACCESS_REMOTE_READ;
                } else if (ltok == 5 &&
                           strncasecmp(fullname, "write", 6) == 0) {
                    type = ACCESS_REMOTE_WRITE;
                } else if (ltok == 7 &&
                           strncasecmp(fullname, "execute", 8) == 0) {
                    type = ACCESS_REMOTE_EXEC;
                }

                free(fullname);

                if (type == ACCESS_REMOTE_INV) {
                    logmsg(LOGMSG_ERROR, 
                            "%s: incorrect access type in string \"%s\"\n",
                            __func__, str);
                    rc = -1;
                    break;
                }

                continue;
            }

            added = 1; /* at least one table identified */
            rc = _access_control_add(acc, type, fullname);
            if (rc) {
                logmsg(LOGMSG_ERROR, "%s: failure to add rights %x to %s\n", __func__,
                        type, fullname);
                free(fullname);
                break;
            }

        } while (1);
    }

    if (created && rc != 0) {
        /* prevent a failed access set to deny everything */
        int irc = fdb_access_control_destroy(clnt);
        if (irc) {
            logmsg(LOGMSG_ERROR, "%s: failed to destroy fdb access control rc=%d\n",
                    __func__, irc);
        }
    }

    return rc;
}

/**
 * Destroy remote access control for a client
 *
 */
int fdb_access_control_destroy(struct sqlclntstate *clnt)
{
    fdb_access_t *acc = clnt->fdb_state.access;

    if (!acc)
        return 0;

    _access_control_clear(acc);

    hash_free(acc->wlst_bydbname);
    hash_free(acc->wlst_byfullname);

    free(acc);

    clnt->fdb_state.access = NULL;

    return 0;
}

/**
 * Check access control for this cursor
 * Returns -1 if access control enabled and access denied
 *         0 otherwise
 *
 */
int fdb_access_control_check(fdb_access_t *acc, const char *dbname,
                             const char *tblname, enum fdb_access_type type)
{
    fdb_access_elem_t *it;
    char *fullname;

    if (!acc)
        return 0;

    it = hash_find_readonly(acc->wlst_bydbname, &dbname);
    if (it) {
        if (it->type & type)
            return 0;
        /* per db not found desired mode, continue */
    }

    fullname = malloc(strlen(dbname) + strlen(tblname) + 2);
    if (!fullname) {
        logmsg(LOGMSG_ERROR, "%s: malloc!\n", __func__);
        return -1;
    }
    sprintf(fullname, "%s.%s", dbname, tblname);

    it = hash_find_readonly(acc->wlst_byfullname, &fullname);

    free(fullname);

    if (it && (it->type & type))
        return 0;

fail:
    access_violations++;
    return -1;
}

static int _access_control_add(fdb_access_t *acc, enum fdb_access_type type,
                               char *fullname /*malloced*/)
{
    fdb_access_elem_t *it, *tmp;

    it = calloc(1, sizeof(*it));
    it->type = type;
    it->fullname = fullname;

    it->tblname = strchr(it->fullname, '.');
    if (it->tblname) {
        if (it->tblname[1] == '\0') {
            it->dbname = strdup(fullname);
            it->tblname = NULL;
        } else {
            it->tblname[0] = '\0';
            it->dbname = strdup(it->fullname);
            it->tblname[0] = '.';
            it->tblname++;
        }
    } else {
        it->dbname = strdup(fullname);
    }

    /* does it already exist ? */
    tmp = hash_find_readonly(acc->wlst_bydbname, &it->dbname);
    if (tmp) {
        tmp->type |= type; /* maybe redundant */
    free:
        free(it->dbname);
        free(it->fullname);
        free(it);
        return 0;
    }

    if (!it->tblname) {
        /* add per db rights; ignore per table dups in the _byfullname hash */
        hash_add(acc->wlst_bydbname, it);
        listc_abl(&acc->lst, it);
        return 0;
    }

    tmp = hash_find_readonly(acc->wlst_byfullname, &it->fullname);
    if (tmp) {
        tmp->type |= type; /* maybe redundant */
        goto free;
    }

    hash_add(acc->wlst_byfullname, it);
    listc_abl(&acc->lst, it);

    return 0;
}

static int _access_control_clear(fdb_access_t *acc)
{
    fdb_access_elem_t *tmp, *it;

    LISTC_FOR_EACH_SAFE(&acc->lst, it, tmp, lnk)
    {
        if (it->tblname) {
            hash_del(acc->wlst_byfullname, it);
        } else {
            hash_del(acc->wlst_bydbname, it);
        }

        listc_rfl(&acc->lst, it);

        if (it->dbname)
            free(it->dbname);
        if (it->fullname)
            free(it->fullname);
        free(it);
    }

    return 0;
}

