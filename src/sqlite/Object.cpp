#include "object.h"

using namespace sqlite;

Object::Object(Database *database)
{
    this->database = database;
}

Object::~Object()
{

}
