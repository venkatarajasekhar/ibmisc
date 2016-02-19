// https://github.com/google/googletest/blob/master/googletest/docs/Primer.md

#include <gtest/gtest.h>
#include <ibmisc/indexing.hpp>
#include <ibmisc/IndexSet.hpp>
#include <iostream>
#include <cstdio>
#include <memory>
#include <map>

using namespace ibmisc;
using namespace netCDF;

// The fixture for testing class Foo.
class IndexingTest : public ::testing::Test {
protected:
	std::vector<std::string> tmpfiles;

	// You can do set-up work for each test here.
	IndexingTest() {}

	// You can do clean-up work that doesn't throw exceptions here.
	virtual ~IndexingTest()
	{
		for (auto ii(tmpfiles.begin()); ii != tmpfiles.end(); ++ii) {
			::remove(ii->c_str());
		}
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	// Code here will be called immediately after the constructor (right
	// before each test).
	virtual void SetUp() {}

	// Code here will be called immediately after each test (right
	// before the destructor).
	virtual void TearDown() {}

//	  // The mock bar library shaed by all tests
//	  MockBar m_bar;
};

TEST_F(IndexingTest, indexing_column_major_test)
{
	Indexing<int, long> ind(
		{0,0},		// Base
		{5,4},		// Extent
		{1,0});		// Column major
	std::array<int,2> tuple;

	tuple = {3,2};
	long ix2 = ind.tuple_to_index<2>(tuple);
	std::array<int,2> tuple2 = ind.index_to_tuple<2>(ix2);

	EXPECT_EQ(20, ind.size());
	EXPECT_EQ(13, ix2);
	EXPECT_EQ(tuple, tuple2);

}

TEST_F(IndexingTest, indexing_row_major_test)
{
	Indexing<int, long> ind(
		{0,0},		// Base
		{4,5},		// Extent
		{0,1});		// Row major
	std::array<int,2> tuple;

	tuple = {3,2};
	long ix2 = ind.tuple_to_index<2>(tuple);
	std::array<int,2> tuple2;
	tuple2 = ind.index_to_tuple<2>(ix2);

	EXPECT_EQ(20, ind.size());
	EXPECT_EQ(17, ix2);
	EXPECT_EQ(tuple, tuple2);

}

TEST_F(IndexingTest, indexing_netcdf)
{
	std::string fname("__netcdf_indexing_test.nc");
	tmpfiles.push_back(fname);
	::remove(fname.c_str());

	NcIO ncio(fname, NcFile::replace);

	Indexing<int, long> ind(
		{0,0},		// Base
		{4,5},		// Extent
		{0,1});		// Row major
	ind.ncio(ncio, ncInt, "indexing");	

	ncio.close();
}

// -----------------------------------------------------------
TEST_F(IndexingTest, index_set)
{
	IndexSet<std::string> names;

	names.insert("A");
	names.insert("B");
	names.insert("C");

	EXPECT_EQ("A", names.at(0));
	EXPECT_EQ("B", names.at(1));
	EXPECT_EQ("C", names.at(2));

	EXPECT_EQ(0, names.at("A"));
	EXPECT_EQ(1, names.at("B"));
	EXPECT_EQ(2, names.at("C"));

	auto ii(names.begin());
	EXPECT_EQ(names.at(0), *ii);
	++ii;
	EXPECT_EQ(names.at(1), *ii);
	++ii;
	EXPECT_EQ(names.at(2), *ii);
	++ii;
	EXPECT_EQ(names.end(), ii);

	EXPECT_TRUE(names.contains("A"));
	EXPECT_FALSE(names.contains("ZZTOP"));
	EXPECT_THROW(names.at("ZZTOP"), ibmisc::Exception);
	EXPECT_THROW(names.at(17), ibmisc::Exception);
}


// -----------------------------------------------------------


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
