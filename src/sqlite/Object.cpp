#include "Object.h"

using namespace sqlite;

Object::Object(Database *database)
{
    this->database = database;
}

Object::~Object()
{

}
