#include "gtest/gtest.h"
#include <Catalogue.h>
#include <Event.h>
#include <JsonIO.h>

namespace
{
  class ACatalogue : public ::testing::Test
  {
  protected:
    ACatalogue() {}

    virtual ~ACatalogue() {}
  };

  TEST(ACatalogue, CanBeSavedToJson) {}

  TEST(ACatalogue, CaBeLoadedFromJson) {}

} // namespace

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
