#pragma once

class Render;

namespace bugattack
{

namespace tower {
class TowerBase;
class Gun;
class Laser;
class RocketGun;
}

namespace ammo {
class Shot;
class LaserRay;
class Rocket;
}

class Map;

void Render(class Render& render, const Map& map);

void Render(class Render& render, const ammo::Shot& shot);

void Render(class Render& render, const ammo::LaserRay& ray);

void Render(class Render& render, const ammo::Rocket& rocket);

void Render(class Render& render, const tower::Gun& gun);

void Render(class Render& render, const tower::Laser& laser);

void Render(class Render& render, const tower::RocketGun& rocket_gun);

void Render(class Render& render, const tower::TowerBase& tower);

void Render(class Render& render, const Map& map);

}
