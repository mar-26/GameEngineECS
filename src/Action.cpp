#include "../include/Action.hpp"

#include <sstream>

Action::Action()
{
}

Action::Action(const std::string& name, const std::string& type)
    : m_name(name), m_type(type)
{

}

Action::Action(const std::string& name, const std::string& type, const Vector& pos)
    : m_name(name), m_type(type), m_pos(pos)
{

}

Action::Action(const std::string& name, const std::string& type, const Vector& pos, const int& delta)
    : m_name(name), m_type(type), m_pos(pos), m_delta(delta)
{

}

const std::string& Action::name() const
{
    return m_name;
}

const std::string& Action::type() const
{
    return m_type;
}

const Vector& Action::pos() const
{
    return m_pos;
}

const int& Action::delta() const
{
    return m_delta;
}

std::string Action::toString() const
{
    std::stringstream ss;
    ss << name() << " " << type();
    return ss.str();
}