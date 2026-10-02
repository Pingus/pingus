// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <sstream>

#include "pingus/object_schema.hpp"

using namespace pingus;

namespace {

ObjectData read_object(std::string const& text)
{
  ReaderDocument doc = ReaderDocument::from_string(text);
  ReaderObject root = doc.get_root();
  ObjectTypeDef const* type = ObjectSchema::instance().find(root.get_name());
  if (!type) {
    throw std::runtime_error("unknown type: " + root.get_name());
  }
  return ObjectData::from_reader(*type, root.get_name(), root.get_mapping());
}

std::string write_object(ObjectData const& data)
{
  std::ostringstream out;
  {
    Writer writer = Writer::from_stream(prio::Format::SEXPR, out);
    data.write(writer);
  }
  return out.str();
}

} // namespace

TEST(ObjectSchemaTest, find_by_name_and_alias)
{
  ObjectSchema const& schema = ObjectSchema::instance();
  ASSERT_NE(schema.find("spike"), nullptr);
  EXPECT_EQ(schema.find("snow"), schema.find("snow-generator"));
  EXPECT_EQ(schema.find("does-not-exist"), nullptr);
}

TEST(ObjectSchemaTest, defaults_for_missing_properties)
{
  ObjectData data = read_object("(entrance (position 10 20 30))");
  EXPECT_EQ(data.get_pos(), Vector2f(10, 20));
  EXPECT_EQ(data.get_z_index(), 30.0f);
  EXPECT_EQ(data.get<int>("release-rate"), 150);
  EXPECT_EQ(data.get<std::string>("direction"), "misc");
  EXPECT_EQ(data.get<int>("owner-id"), 0);
}

TEST(ObjectSchemaTest, read_values_and_aliases)
{
  ObjectData data = read_object("(iceblock (position 1 2 0) (width 7))");
  EXPECT_EQ(data.get<int>("repeat"), 7);

  ObjectData exit = read_object("(exit (position 1 2 0) (owner-id 2)"
                                " (surface (image \"exit/igloo\") (modifier \"ROT90\")))");
  EXPECT_EQ(exit.get<int>("owner-id"), 2);
  EXPECT_EQ(exit.get<ResDescriptor>("surface").res_name, "exit/igloo");
  EXPECT_EQ(exit.get<ResDescriptor>("surface").modifier, ResourceModifier::ROT90);
}

TEST(ObjectSchemaTest, color_fallback)
{
  ObjectData colori = read_object("(solidcolor-background (colori 10 20 30 255))");
  EXPECT_EQ(colori.get<Color>("colori"), Color(10, 20, 30, 255));

  ObjectData color = read_object("(solidcolor-background (color 1.0 0.0 0.0 1.0))");
  EXPECT_EQ(color.get<Color>("colori"), Color(255, 0, 0, 255));
}

TEST(ObjectSchemaTest, missing_position_uses_default_z)
{
  ObjectData data = read_object("(surface-background (surface (image \"textures/lunartile\") (modifier \"ROT0\")))");
  EXPECT_EQ(data.get_pos(), Vector2f(0, 0));
  EXPECT_EQ(data.get_z_index(), -150.0f);
  EXPECT_FLOAT_EQ(data.get<float>("para-x"), 0.5f);
}

TEST(ObjectSchemaTest, write_order_and_alias_name)
{
  ObjectData data = read_object("(snow (position 1 2 3) (intensity 2.5))");
  std::string const text = write_object(data);
  EXPECT_EQ(text.find("(snow"), 0u) << text;
  EXPECT_NE(text.find("(intensity 2.5)"), std::string::npos) << text;

  ObjectData gp = read_object("(groundpiece (position 1 2 3) (type \"solid\")"
                              " (surface (image \"groundpieces/ground/misc/block\") (modifier \"ROT0\")))");
  std::string const gp_text = write_object(gp);
  // type and surface come before the position
  EXPECT_LT(gp_text.find("(type"), gp_text.find("(surface"));
  EXPECT_LT(gp_text.find("(surface"), gp_text.find("(position"));
}

TEST(ObjectSchemaTest, set_and_type_mismatch)
{
  ObjectData data(*ObjectSchema::instance().find("conveyorbelt"));
  data.set("speed", 3);
  EXPECT_EQ(data.get<int>("speed"), 3);
  EXPECT_THROW(data.set("speed", 1.5f), std::runtime_error);
  EXPECT_THROW(data.get<int>("no-such-property"), std::runtime_error);
}

/* EOF */
