#include "serializable.h"

Serializable::~Serializable()
{
}


QMap<QString, Serializable * (*)()> SerializableFactory::m_tagmap;
