#include "gtest/gtest.h"
#include <Catalogue.h>
#include <Event.h>

namespace
{
  class AnEvent : public ::testing::Test
  {
  protected:
    AnEvent() {}

    virtual ~AnEvent() {}
  };

  TEST(AnEvent, CanBeDefaultConstructed)
  {
    auto e = CatalogiCpp::Event<double>();
    EXPECT_FALSE(e.uuid.is_nil());
  }

  TEST(AnEvent, CanBeConstructed)
  {
    auto e =
        CatalogiCpp::Event<double>{"Event1", {}, {}, CatalogiCpp::make_uuid()};
    EXPECT_FALSE(e.uuid.is_nil());

    EXPECT_EQ(e.name, "Event1");
  }

  TEST(AnEvent, IsUnique)
  {
    auto e1 = CatalogiCpp::Event<double>();
    auto e2 = CatalogiCpp::Event<double>();
    EXPECT_NE(e1.uuid, e2.uuid);
  }

  TEST(AnEvent, CanFindStartTime)
  {
    using Event_t = CatalogiCpp::Event<double>;
    auto e        = Event_t{"Event1",
                     {},
                     {Event_t::Product_t{"Product1", 10., 11.},
                      Event_t::Product_t{"Product2", 10., 11.},
                      Event_t::Product_t{"Product3", 10.2, 11.}},
                     CatalogiCpp::make_uuid()};
    EXPECT_EQ(10., e.startTime());
  }

  TEST(AnEvent, CanFindStopTime)
  {
    using Event_t = CatalogiCpp::Event<double>;
    auto e        = Event_t{"Event1",
                     {},
                     {Event_t::Product_t{"Product1", 10., 11.},
                      Event_t::Product_t{"Product2", 10., 12.},
                      Event_t::Product_t{"Product3", 10.2, 11.}},
                     CatalogiCpp::make_uuid()};
    EXPECT_EQ(12., e.stopTime());
  }
} // namespace

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
