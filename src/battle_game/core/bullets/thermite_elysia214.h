#pragma once
#include "battle_game/core/bullet.h"

namespace battle_game::bullet {
class Thermite214 : public Bullet {
 public:
  Thermite214(GameCore *core,
             uint32_t id,
             uint32_t unit_id,
             uint32_t player_id,
             glm::vec2 position,
             float rotation,
             float damage_scale,
             glm::vec2 velocity);
  ~Thermite214() override;
  void Render() override;
  void Update() override;
  int status_=0;
  uint32_t unit_hit=0xffffffffu;
  uint32_t damage_count_down_=0;
  uint32_t damage_count_=0;
  float damage_range_=4.0f;
 private:
  glm::vec2 velocity_{};
};
}  // namespace battle_game::bullet
