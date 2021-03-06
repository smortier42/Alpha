#include "../inc/algocoll.hpp"

bool move_car(World *env, std::vector<Car> *Lot, int cars_placed, std::vector<Car> *map)
{
	int checkp_ret = 0;
	int collision_ret = car_collision((*Lot)[cars_placed], (*Lot)[cars_placed].m_coords.size(), map);
	bool collision_trigger = false;
	//ici mettre un switch (boolean) et dans la boucle on avance une fois sur deux (pourquoi ? - maybe symétrie)

	reset_angle(&(*Lot)[cars_placed]);
	while (collision_ret)
	{
		collision_trigger = true;
		DEBUGP printf("\e[31m\tCan't place Car%d at pos %f\e[39m\n", cars_placed, (*Lot)[cars_placed].m_shift);
		checkp_ret = check_params(env, &(*Lot)[cars_placed]);
		if (!checkp_ret)
		{
			reset_angle(&(*Lot)[cars_placed]);
			if ((*Lot)[cars_placed].m_shift + collision_ret < env->GetLimiteCamion() && collision_ret > (*Lot)[cars_placed].m_shift)
				(*Lot)[cars_placed].m_shift = collision_ret + 50;
			else
				(*Lot)[cars_placed].m_shift += env->GetStep();
			if ((*Lot)[cars_placed].m_lenght + (*Lot)[cars_placed].m_shift >= env->GetLimiteCamion())
			{
				DEBUGP printf("\e[31m\t\tLeave cause of the limite\e[39m\n");
				return false;
			}
		}
		collision_ret = car_collision((*Lot)[cars_placed], (*Lot)[cars_placed].m_coords.size(), map);
	}
	if ((*Lot)[cars_placed].m_shift + (*Lot)[cars_placed].m_lenght >= env->GetLimiteCamion())
	{
		DEBUGP printf("\e[31m\t\tLeave cause of the limite but wout collision\e[39m\n");
		return false;
	}
	if (!collision_trigger) // If the map is empty (AKA first car), but no more collide with the same car
	{
		reset_angle(&(*Lot)[cars_placed]);
		if (!check_params(env, &(*Lot)[cars_placed]))
			(*Lot)[cars_placed].m_shift += (*env).GetStep();
	}
	return true;
}

int place_cars(World *env, std::vector<Car>* Lot, int number_cars, std::vector<Car>* map, int index)
{
	static int cars_placed = 0;
	static int number_call;

	while (cars_placed != number_cars)
	{
		number_call++;
		DEBUGP printf("Number call = %d\n", number_call);
		DEBUGP std::cout << "\e[34mPlacing = " << cars_placed << "\e[39m" << std::endl;
		if (move_car(env, Lot, cars_placed, map)) // we move car_placed car following the route until it can be placed
		{
			(*map).push_back((*Lot)[cars_placed]);
			DEBUGP printf("\t\t\e[32mCar%d added on the map at %f\e[39m\n", cars_placed, (*Lot)[cars_placed].m_shift);
			cars_placed += 1;
		}
		else
		{
			if (cars_placed == 0)
				return (-1);
			(*map).pop_back();
			DEBUGP printf("\e[31m\tCar%lu as been deleted from the map\e[39m\n", (*map).size());
			index = 0;
			(*Lot)[cars_placed].m_shift = 0.0f;
			reset_angle(&(*Lot)[cars_placed]);
			cars_placed -= 1;
			if ((*map).size() <= 0 && (*Lot)[cars_placed].m_lenght + (*Lot)[cars_placed].m_shift >= env->GetLimiteCamion())
				return -1;
			if (cars_placed != 0)
				(*Lot)[cars_placed].m_shift += env->GetStep();
			index = 0;
			for (auto tmp : (*Lot)[cars_placed].m_coords)
			{
				(*Lot)[cars_placed].m_coords[index].x  = (*Lot)[cars_placed].m_coords_init[index].x + (*Lot)[cars_placed].m_shift;
				index++;
			}
		}
		VISUP{
				for (auto tmp : (*map))
				{
					std::cout << "------" << std::endl;
					for (auto point : tmp.m_coords)
					{
						std::cout << point.x <<"||"<< point.y << std::endl;
					}
				}
				std::cout << "END MAP" << std::endl;
			}
	}
	return (0);
}
