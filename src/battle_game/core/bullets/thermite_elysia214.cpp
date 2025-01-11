#include "battle_game/core/bullets/thermite_elysia214.h"

#include "battle_game/core/game_core.h"
#include "battle_game/core/particles/particles.h"

namespace battle_game::bullet {
Thermite214::Thermite214(GameCore *core,
                       uint32_t id,
                       uint32_t unit_id,
                       uint32_t player_id,
                       glm::vec2 position,
                       float rotation,
                       float damage_scale,
                       glm::vec2 velocity)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
      velocity_(velocity) {
}

const int NORMAL=0, HIT=1, EXPLODE=2;

void Thermite214::Render() {
  SetTransformation(position_, rotation_, glm::vec2{0.1f});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/particle3.png");
  DrawModel(0);
}

void Thermite214::Update() {
    if (status_==NORMAL){
        position_ += velocity_ * kSecondPerTick;
        if (game_core_->IsBlockedByObstacles(position_)) {
            status_=EXPLODE;
            damage_count_=5;
        }
        auto &units = game_core_->GetUnits();
        for (auto &unit : units) {
            if (unit.first == unit_id_) {
                continue;
            }       
            if (unit.second->IsHit(position_)) {
                unit_hit=unit.first;
                status_=HIT;
                damage_count_=5;
            }
        } 
    }else if (status_==HIT){
        if (damage_count_down_==0){
            game_core_->PushEventDealDamage(unit_hit, unit_id_, damage_scale_ * 5.0f);
            damage_count_--;
            damage_count_down_=0.5f*kTickPerSecond;
            for (int i = 0; i < 5; i++) {
                game_core_->PushEventGenerateParticle<particle::Smoke>(
                    position_, rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
                    glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
            }
        }else{
            damage_count_down_--;
        }
        if (damage_count_==0){
            game_core_->PushEventRemoveBullet(id_);
        }
    }else if (status_==EXPLODE){
        if (damage_count_down_==0){
            auto &units = game_core_->GetUnits();
            for (auto & unit : units) {
                auto dis = glm::distance(unit.second-> GetPosition(), position_);
               if (dis <= damage_range_) {
                    game_core_->PushEventDealDamage(unit.first, id_, damage_scale_ * 10.0f * (1.0 - pow(dis / damage_range_ , 2)));
                }   
            }
            damage_count_down_=0.5f*kTickPerSecond;
            damage_count_--;    
            game_core_->PushEventGenerateParticle<particle::Smoke>(
                position_, rotation_, game_core_->RandomInCircle() * 2.0f, 4.0f,
                glm::vec4{1.0f, 1.0f, 0.0f, 0.5f}, 3.0f);
        }else{
            damage_count_down_--;
        }
        if (damage_count_==0){
            game_core_->PushEventRemoveBullet(id_);
        }
    }
  
  

  

 
}

Thermite214::~Thermite214() {
  for (int i = 0; i < 5; i++) {
    game_core_->PushEventGenerateParticle<particle::Smoke>(
        position_, rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
  }
}
}  // namespace battle_game::bullet
