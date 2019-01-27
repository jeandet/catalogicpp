#include "gtest/gtest.h"
#include <Catalogue.h>
#include <Event.h>

namespace
{
  class ACatalogue : public ::testing::Test
  {
  protected:
    ACatalogue() {}

    virtual ~ACatalogue() {}
  };

  TEST(ACatalogue, CanBeDefaultConstructed)
  {
    auto c = CatalogiCpp::Catalogue<double>();
    EXPECT_FALSE(c.uuid.is_nil());
  }

  TEST(ACatalogue, CanBeConstructed)
  {
    auto c = CatalogiCpp::Catalogue<double>{
        "Catastrophic", {}, CatalogiCpp::make_uuid()};
    EXPECT_FALSE(c.uuid.is_nil());
    EXPECT_EQ("Catastrophic", c.name);
  }

} // namespace

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
