#include "ecs.hpp"

// libs
#include <glm/glm.hpp>

// std
#include <sys/time.h>

#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

struct Transform
{
  glm::vec3 translation;
  glm::vec3 rotation;
  float scale;
};

struct RigidBody
{
  glm::vec3 position;
  glm::vec3 velocity;
};

struct PointLight
{
  glm::vec3 color;
  float intensity;
};

void testAddRemove()
{
  ecs::EntManager em{};
  for (int i = 1; i <= 8; ++i)
  {
    auto entId = em.createEnt();
    em.add<Transform>(entId);
    if (i % 2 == 0)
    {
      em.add<RigidBody>(entId);
    }
    if (i % 4 == 0)
    {
      em.add<PointLight>(entId);
    }
  }

  auto gameObjects = em.getAllEnts();
  assert(gameObjects.ids().size() == 8 && "Expected 8 ents");

  auto newEntId = em.createEnt();
  assert(gameObjects.ids().size() == 9 && "Expected 9 ents");

  auto query = em.allOf<Transform, RigidBody>();
  assert(query.ids().size() == 4 && "Expected 4 ents");

  em.add<Transform, RigidBody>(newEntId);

  assert(query.ids().size() == 5 && "Expected 5 ents");

  em.remove<RigidBody>(newEntId);
  assert(query.ids().size() == 4 && "Expected 4 ents");

  // create copy
  auto toRemove = query.ids();
  for (auto entId : toRemove)
  {
    em.remove<Transform, RigidBody>(entId);
  }

  assert(query.ids().size() == 0 && "Expected 0 ents");
}

void testEmptyQuery()
{
  ecs::EntManager em{};

  auto emptyTest = em.allOf<Transform, RigidBody>();
  for (auto &ent : emptyTest)
  {
  }
  assert(emptyTest.ids().size() == 0 && "Empty query should have no results");
}

void testCompoundQuery()
{
  ecs::EntManager em{};
  // 5 with everything
  for (int i = 0; i < 5; ++i)
    em.add<Transform, RigidBody, PointLight>(em.createEnt());
  // 4 more with transform and rb
  for (int i = 0; i < 4; ++i)
    em.add<Transform, RigidBody>(em.createEnt());
  // 3 with just transform
  for (int i = 0; i < 3; ++i)
    em.add<Transform>(em.createEnt());
  // 5 with just strings
  for (int i = 0; i < 5; ++i)
    em.add<std::string>(em.createEnt());

  {
    auto q = em.anyOf<RigidBody, PointLight>();
    assert(q.size() == 9);
  }

  assert(em.anyOf<Transform>().size() == 12);

  assert(em.noneOf<RigidBody>().size() == 8);

  {
    auto q = em.query().anyOf<Transform, std::string>().noneOf<PointLight>().result();
    assert(q.size() == 12);
  }
}

void testQueryRef()
{
  ecs::EntManager em{};

  for (int i = 1; i < 8; i++)
  {
    auto entId = em.createEnt();
    em.add<Transform>(entId);
    if (i % 2 == 0)
    {
      em.add<RigidBody>(entId);
    }
  }

  auto query = em.allOf<Transform, RigidBody>();
  // matching id test
  {
    auto q2 = em.allOf<Transform, RigidBody>();
    assert(q2.id == query.id && "Queries are the same but don't have matching ids");
  }

  // ref count testing
  auto &queryRef = query;
  {
    auto queryCopy = ecs::EntQueryResult{query};
    auto queryRefCopy = ecs::EntQueryResult{queryRef};
    auto queryMove = ecs::EntQueryResult{std::move(query)};
  }
}

struct C0
{
  float data[32]; // equivalent to two mat4s in space
};

struct C1
{
  float data[32]; // equivalent to two mat4s in space
};

struct C2
{
  float data[32]; // equivalent to two mat4s in space
};

struct C3
{
  float data[32]; // equivalent to two mat4s in space
};

struct C4
{
  float data[32]; // equivalent to two mat4s in space
};

struct C5
{
  float data[32]; // equivalent to two mat4s in space
};

struct C6
{
  float data[32]; // equivalent to two mat4s in space
};

struct C7
{
  float data[32]; // equivalent to two mat4s in space
};

struct C8
{
  float data[32]; // equivalent to two mat4s in space
};

struct C9
{
  float data[32]; // equivalent to two mat4s in space
};

void printTime(const std::string &testName, long long timeDelta)
{
  std::cout << testName << " - Completed in " << timeDelta << "ms" << std::endl;
}

void setTime(long long &timer)
{
  timer = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void stressTest()
{
  long long startTime, endTime;

  {
    ecs::EntManager em{};
    setTime(startTime);
    // 20 indexes
    // 10 components
    int entCountPerType = 10000;
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C2, C3>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C2, C4>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C2, C5>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C6, C9>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C6, C7, C8>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C6>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C6, C9>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C2, C5, C8>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C3, C5, C6>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C2, C3, C4, C5, C6, C7, C8, C9>(em.createEnt());
    setTime(endTime);
    printTime("Create ents with 0 indexes", endTime - startTime);

    setTime(startTime);
    auto query = em.query().allOf<C0, C1>().anyOf<C6, C8>().noneOf<C3>().result();
    setTime(endTime);
    printTime("Non indexed Compound query", endTime - startTime);
  }

  {
    ecs::EntManager em{};

    auto query = em.query().allOf<C0, C1>().anyOf<C6, C8>().noneOf<C3>().result();
    auto q1 = em.allOf<C0, C1>();
    auto q2 = em.allOf<C0, C4>();
    auto q3 = em.allOf<C0, C6>();
    auto q4 = em.allOf<C0, C7>();
    auto q5 = em.allOf<C0, C9>();
    auto q6 = em.anyOf<C1, C2>();
    auto q7 = em.anyOf<C4, C6, C8>();
    auto q8 = em.anyOf<C1, C3, C9>();
    auto q9 = em.anyOf<C5, C7>();
    auto q10 = em.anyOf<C6, C9>();
    auto q11 = em.noneOf<C1>();
    auto q12 = em.noneOf<C2>();
    auto q13 = em.noneOf<C3>();
    auto q14 = em.noneOf<C6, C9>();
    auto q15 = em.noneOf<C4, C5, C9>();

    setTime(startTime);
    // 20 indexes
    // 10 components
    int entCountPerType = 10000;
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C2, C3>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C2, C4>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C2, C5>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C6, C9>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C6, C7, C8>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C6>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C6, C9>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C2, C5, C8>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C3, C5, C6>(em.createEnt());
    for (int i = 0; i < entCountPerType; ++i)
      em.add<C0, C1, C2, C3, C4, C5, C6, C7, C8, C9>(em.createEnt());
    setTime(endTime);
    printTime("Create ents with 16 indexes", endTime - startTime);

    std::cout << "iterate " << q1.size() << std::endl;
    setTime(startTime);
    for (auto entId : q1.ids())
    {
      C0 &comp = em.get<C0>(entId);
      comp.data[4] = 99.f;
    }
    setTime(endTime);
    printTime("update component using ids", endTime - startTime);

    setTime(startTime);
    for (auto ent : q1)
    {
      ent.get<C0>().data[8] = -1.f;
    }
    setTime(endTime);
    printTime("update component using Iterator", endTime - startTime);

    auto &components = em.getComponentMap<C0>().getValues();
    setTime(startTime);
    for (auto &component : components)
    {
      component.data[2] = 3.f;
    }
    setTime(endTime);
    printTime("iterate component values directly", endTime - startTime);
  }
}

int main()
{
  testAddRemove();
  testEmptyQuery();
  testCompoundQuery();
  testQueryRef();
  std::cout << "Functionality tests completed\n";
  stressTest();
  std::cout << "Performance tests completed\n";

  // TODO Move components, don't copy
  // TODO destroy ent
  // TODO refactor container add/remove to be more in line with cpp
  // (ie insert/erase/emplace)
  // TODO unpack add, auto [c0, c1] = em.add<C0, C1>()
  // TODO const checking, and const EntManager;
  // TODO ent try function -> ptr or nullptr if component does not exist
  // TODO has()
  // change observer would be nice... but wouldn't work with Component& get
  // TODO sorting
  // TODO input iterator
}
