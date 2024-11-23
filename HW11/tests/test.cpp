#include "db/database.h"
#include "db/database_result.h"
#include "db/database_error.h"

#include <gtest/gtest.h>

TEST(Database, TestInsertWrongTableId)
{
    Database db;
    auto     result = db.insert(0, 0, "hello");
    ASSERT_TRUE(result.error().value() == DatabaseError::TableNotFound);
    result = db.insert(3, 0, "hello");
    ASSERT_TRUE(result.error().value() == DatabaseError::TableNotFound);
}

TEST(Database, TestInsertDuplicateValue)
{
    Database db;
    db.insert(1, 0, "hello");
    auto result = db.insert(1, 0, "hello");
    ASSERT_TRUE(result.error().value() == DatabaseError::ValueAlreadyExists);

    db.insert(2, 33, "hello");
    result = db.insert(2, 33, "hello");
    ASSERT_TRUE(result.error().value() == DatabaseError::ValueAlreadyExists);
}

TEST(Database, TestInsertSuccess)
{
    Database db;
    auto     result = db.insert(1, 0, "hello");
    ASSERT_TRUE(!result.error().has_value());
    ASSERT_NE(db.table1_.find(0), db.table1_.cend());

    result = db.insert(1, 1, "hello");
    ASSERT_TRUE(!result.error().has_value());
    ASSERT_NE(db.table1_.find(1), db.table1_.cend());

    result = db.insert(2, 0, "hello");
    ASSERT_TRUE(!result.error().has_value());
    ASSERT_NE(db.table2_.find(0), db.table2_.cend());

    result = db.insert(2, 3, "hello");
    ASSERT_TRUE(!result.error().has_value());
    ASSERT_NE(db.table2_.find(3), db.table2_.cend());
    ASSERT_EQ(db.table1_.size(), 2);
    ASSERT_EQ(db.table2_.size(), 2);
}

TEST(Database, TestTruncate)
{
    Database db;
    db.insert(1, 0, "hello");
    db.insert(1, 1, "hello");
    db.insert(2, 0, "hello");
    db.insert(2, 1, "hello");

    auto result = db.clear(1);
    ASSERT_TRUE(db.table1_.empty());
    ASSERT_EQ(db.table2_.size(), 2);
    ASSERT_FALSE(result.error().has_value());

    result = db.clear(2);
    ASSERT_TRUE(db.table2_.empty());
    ASSERT_FALSE(result.error().has_value());
}

bool vectorsAreEqual(const std::vector<std::string>& v1, const std::vector<std::string>& v2)
{
    return v1 == v2;
}

TEST(Database, TestIntersection)
{
    Database db;
    db.insert(1, 0, "hello");
    db.insert(1, 1, "hello");
    db.insert(2, 0, "hello");
    db.insert(2, 1, "hello");

    auto result = db.intersection();
    ASSERT_EQ(std::distance(result.cbegin(), result.cend()), 2);
    ASSERT_FALSE(result.error().has_value());

    auto iter = result.cbegin();
    ASSERT_EQ(iter->first, 0);
    ASSERT_TRUE(vectorsAreEqual(iter->second, std::vector<std::string> { "hello", "hello" }));

    ++iter;
    ASSERT_EQ(iter->first, 1);
    ASSERT_TRUE(vectorsAreEqual(iter->second, std::vector<std::string> { "hello", "hello" }));
}

TEST(Database, TestSymmetricDifference)
{
    Database db;
    db.insert(1, 0, "hello");
    db.insert(1, 1, "hello");
    db.insert(2, 2, "hello");
    db.insert(2, 3, "hello");

    auto result = db.symmetricDifference();
    ASSERT_EQ(std::distance(result.cbegin(), result.cend()), 4);
    ASSERT_FALSE(result.error().has_value());

    auto iter = result.cbegin();
    ASSERT_EQ(iter->first, 0);
    ASSERT_TRUE(vectorsAreEqual(iter->second, std::vector<std::string> { "hello", "" }));

    ++iter;
    ASSERT_EQ(iter->first, 1);
    ASSERT_TRUE(vectorsAreEqual(iter->second, std::vector<std::string> { "hello", "" }));

    ++iter;
    ASSERT_EQ(iter->first, 2);
    ASSERT_TRUE(vectorsAreEqual(iter->second, std::vector<std::string> { "", "hello" }));

    ++iter;
    ASSERT_EQ(iter->first, 3);
    ASSERT_TRUE(vectorsAreEqual(iter->second, std::vector<std::string> { "", "hello" }));
}