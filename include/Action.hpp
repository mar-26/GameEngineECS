#ifndef ACTION_HPP
#define ACTION_HPP

#include "Entity.hpp"

class Action
{
    public:
        Action();
        Action(const std::string& name, const std::string& type);
        Action(const std::string& name, const std::string& type, const Vector& pos);
        Action(const std::string& name, const std::string& type, const Vector& pos, const int& delta);

        const std::string& name() const;
        const std::string& type() const;
        const Vector& pos() const;
        const int& delta() const;
        std::string toString() const;

    private:
        std::string m_name = "NONE";
        std::string m_type = "NONE";
        Vector m_pos = {};
        int m_delta = 0;
};

#endif