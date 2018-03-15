#include <iostream>

#pragma once


// class TWSAPIDLLEXP SoftDollarTier
// Dont plan on using Soft Dollar Tier anyway
class SoftDollarTier
{

    std::string m_name, m_val, m_displayName;

public:

	    explicit SoftDollarTier(const std::string &m_name = "", const std::string &m_val = "", const std::string &m_displayName = "");

	    std::string name() const;

	    std::string val() const;

	    std::string displayName() const;
};