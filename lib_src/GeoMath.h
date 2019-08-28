#pragma once

#include <math.h>
#include <vector>
#include <ostream>
#include <iostream>
#include <algorithm>


///Возвращает 1, если число >=0, иначе -1
double sign(double arg);

namespace GeoMath
{
	/*!
	@brief Объявление используемых констант
	*/
	static  struct
	{
		const double Pi = 3.1415926535897932384626433832795; ///< число Пи

		const double E_EARTH = 6378137.0;	///< Экваториальный радиус Земли
		const double P_EARTH = 6356779.0;	///< Полярный радиус Земли

		const double E_length = E_EARTH * 2 * Pi;	///< Удвоенная длина меридиана
		const double P_length = P_EARTH * 2 * Pi;	///< Длина экватора

		const double M_IN_LAT = P_length / 360;	///< Число метров в одном градусе широты
		const double M_IN_LNG = E_length / 360;	///< Число метров в одном градусе долготы

		const double DEG2RAD = 0.01745329252;	///< Коэффициент преобразования градусов в радианы
		const double RAD2DEG = 57.2957795129;	///< Коэффициент преобразования градиан в градусы
	} CONST;

	/// Оси
	enum Axis
	{
		X,	///< Направление с Юга на Север
		Y,	///< Направление с Запада на Восток
		Z	///< Направление вверх
	};

	
	/// Вращение задается по правилу правой руки, либо по правилу левой руки
	enum Hand
	{
		LEFT,	///< По правилу левой руки
		RIGHT	///< По правилу правой руки
	};
	
	enum PositionType ///< Указывает, как именно задано смещение либо положение
	{
		OFFSET, ///< Указывает, что положение задано относительно предыдущей точки
		HOME	///< Указывает, что положение задано относительно домашней точки
	};

/// Двумерный вектор
class v2
	{
	public:
		double x;
		double y;

		v2(double x_ = 0, double y_ = 0);

		bool isNull();
		double	length_xy();
		double angle_xy(v2 v);

		v2 normalize_xy(double abs);
		v2 rotateXY(double rad);

		v2 operator+(v2 const& v);
		v2 operator-(v2 const& v);
		v2 operator*(double factor);
		v2 operator/(double factor);
		
		bool operator ==(v2 const& p2);
		
		friend std::ostream& operator<<(std::ostream& os, const GeoMath::v2& at);

	};
	
	class v3
	{
	public:
		double x;
		double y;
		double z;

		v3(double x_ = 0, double y_ = 0, double z_ = 0);
		v3(v2 v);

		bool isNull();
		double	length_xy();
		double	length_xyz();
		double angle_xy(v3 v);

		v3 normalize_xy(double abs);
		v3 normalize_xyz(double abs);
		v3 rotateXY(double rad);

		v3 rotate(double rad, Axis axis,Hand hand=RIGHT);
		v3 rotate(double rad, Axis axis, v3 from_point, Hand hand = RIGHT);

		v3 operator+(v3 const& v2);
		v3 operator-(v3 const& v2);
		v3 operator*(double factor);
		v3 operator/(double factor);
		
		bool operator ==(v3 const& v);
		
		friend std::ostream& operator<<(std::ostream& os, const GeoMath::v3& at);

	};

	
	
	
	class v3geo
	{
	public:
		double lat;
		double lng;
		double alt;
		
		bool isNull();
		
		v3geo(double lat_ = 0, double lng_ = 0, double alt_ = 0);

		v3 operator-(v3geo const& v2);
		v3geo operator+(v3 const& v2);
		bool operator ==(v3geo const& p2);
		
		friend std::ostream& operator<<(std::ostream& os, const GeoMath::v3geo& at);
	};

	class RouteTemplate2D
	{
	public:
		
		enum TemplateState
		{
			NOT_SELECT,
			ABSOLUT,
			METERS
		};
		
		
		struct Position
		{
			GeoMath::v2 offset;
			GeoMath::v2 home;
			GeoMath::v3geo abs;
		};
		
		RouteTemplate2D();
		RouteTemplate2D(const RouteTemplate2D& route);
		~RouteTemplate2D();
		
		void add_next(PositionType position,v2 point);
		
		TemplateState state;
		Position at(int i);
		int size();
		Position operator[](const int i);
		bool set_reference_points(v3geo abs1, int index_1, v3geo abs2, int index_2);
		bool set_reference_points(v2 p1, int index_1, v2 p2, int index_2);
		
		GeoMath::v2 get_home_meters();
	private:
		
		GeoMath::v3geo home_abs;
		GeoMath::v2 home_meters;
		
		GeoMath::v3geo reference_point_1_abs;
		GeoMath::v3geo reference_point_2_abs;
		
		GeoMath::v3 reference_point_1_meters;
		GeoMath::v3 reference_point_2_meters;
		
		float course;
		float scale;
		

		std::vector<GeoMath::v2> point_offset;
		std::vector<GeoMath::v2> point_home;
		
		
	};


	
	class SimpleFigure3D
	{
	public:
		
		struct Position
		{
			GeoMath::v3 offset;
			GeoMath::v3 home;
		};
		
		SimpleFigure3D();
		~SimpleFigure3D();
		
		void add_next(PositionType position, v3 point);
		Position at(int i);		
		int size();
		void rotate(double rad, Axis axis, v3 from_point, Hand hand = RIGHT);
		Position operator[](const int i);
		
		GeoMath::v3 center();

	private:
		std::vector<GeoMath::v3> point_offset;
		std::vector<GeoMath::v3> point_home;
	};

	class RouteLine
	{
	public:
		enum PositionType
		{
			HOME,
			OFFSET
		};
		
		std::vector<v3> points;//points in Centre notion
		bool add_next(v3 point, PositionType notion);
		v3 at(unsigned long i, PositionType notion);
		std::vector<v3> get_points();
		bool rotate(double rad, Axis axis, Hand hand = RIGHT);
		std::vector<v3geo> absPosition(v3geo home);
		RouteLine();
		~RouteLine();

	private:

	};



}

