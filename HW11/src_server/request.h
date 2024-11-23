#pragma once

#include "db/database.h"

class Request
{
public:
    virtual ~Request() = default;

    virtual DatabaseResult applyToDatabase(Database& database) = 0;
};

class RequestInsert : public Request
{
    friend class RequestFactory;
public:
    DatabaseResult applyToDatabase(Database& database) override { return database.insert(tableId_, id_, value_); }

private:
    std::size_t tableId_ = 0;
    int         id_      = 0;
    std::string value_;
};

class RequestClear : public Request
{
    friend class RequestFactory;

public:
  DatabaseResult applyToDatabase(Database& database) { return database.clear(tableId_); }

private:
    std::size_t tableId_ = 0;
};

class RequestIntersection : public Request
{
    friend class RequestFactory;

public:
    DatabaseResult applyToDatabase(Database& database) { return database.intersection(); }
};

class RequestSymmetricDifference : public Request
{
    friend class RequestFactory;

public:
    DatabaseResult applyToDatabase(Database& database) { return database.symmetricDifference(); }
};