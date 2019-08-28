#include "GeoMath.h"

/*!
@brief возвращает 1, если число >=0, иначе -1
 */
double
sign(double arg)
{
	return arg < 0 ? -1 : 1;
}


template<typename T, size_t M, size_t N>
	void arr2copy(const T from[M][N], T (&to)[M][N])
	{
		for (size_t m = 0; m < M; m++)
		{
			for (size_t n = 0; n < N; n++)
			{
				to[m][n] = from[m][n];
			}
		}
	}

GeoMath::v3 GeoMath::v3::rotate(double rad,  Axis axis, v3 c, Hand hand)
{	
	v3 res(0, 0, 0);
	switch (axis)
	{	
	case GeoMath::Axis::X:
		res.x = this->x;
		res.y = c.y + (this->y - c.y)*cos(rad) + (this->z - c.z)*sin(rad);
		res.z = c.z - (this->y - c.y)*sin(rad) + (this->z - c.z)*cos(rad);
		break;
		
	case GeoMath::Axis::Y:
		res.y = this->y;
		res.x = c.x + (this->x - c.x)*cos(rad) - (this->z - c.z)*sin(rad);
		res.z = c.z + (this->x - c.x)*sin(rad) + (this->z - c.z)*cos(rad);
		break;
		
	case GeoMath::Axis::Z:
		res.z = this->z;
		res.x = c.x + (this->x - c.x)*cos(rad) + (this->y - c.y)*sin(rad);
		res.y = c.y - (this->x - c.x)*sin(rad) + (this->y - c.y)*cos(rad);
		break;
	}		
	this->x = res.x;
	this->y = res.y;
	this->z = res.z;
}


GeoMath::v2::v2(double x_, double y_)
	: x(x_)
	, y(y_)
{
}

bool
GeoMath::v2::isNull()
{

	return (x == 0 && y == 0);
}

double
GeoMath::v2::length_xy()
{
	double result = sqrt(x * x + y * y);
	return result;
}


double
GeoMath::v2::angle_xy(v2 v = v2(1, 0))
{
	double sgn;
	double value;
	if (this->isNull() || v.isNull())
		return 0;

	value = this->x * v.x + this->y * v.y;
	value = value / (this->length_xy() * v.length_xy());
	sgn = -sign(this->x * v.y - this->y * v.x);

	return sgn * acos(value);
}

GeoMath::v2
GeoMath::v2::normalize_xy(double abs = 1)
{
	if (this->isNull())
		return v2(0, 0);

	v2     result;
	double factor = abs / this->length_xy();

	result.x = this->x * factor;
	result.y = this->y * factor;

	return result;
}


GeoMath::v2
GeoMath::v2::rotateXY(double rad)
{
	v2 result(this->x * cos(rad) - this->y * sin(rad),
		this->x * sin(rad) + this->y * cos(rad));
	return result;
}







GeoMath::v3::v3(double x_, double y_, double z_)
	: x(x_)
	, y(y_)
	, z(z_)
{
}

GeoMath::v3::v3(v2 v)
	: x(v.x)
	, y(v.y)
	, z(0)
{
}

GeoMath::v3geo::v3geo(double lat_, double lng_, double alt_)
	: lat(lat_)
	, lng(lng_)
	, alt(alt_) {};

bool
GeoMath::v3::isNull()
{

	return (x == 0 && y == 0 && z==0);
}

double
GeoMath::v3::length_xy()
{
	double result = sqrt(x * x + y * y);
	return result;
}

double
GeoMath::v3::length_xyz()
{
	double result = sqrt(x * x + y * y + z * z);
	return result;
}

double
GeoMath::v3::angle_xy(v3 v = v3(1, 0, 0))
{
	double sgn;
	double value;
	if (this->isNull() || v.isNull())
		return 0;

	value = this->x * v.x + this->y * v.y;
	value = value / (this->length_xy() * v.length_xy());
	sgn = -sign(this->x * v.y - this->y * v.x);

	return sgn * acos(value);
}

GeoMath::v3
GeoMath::v3::normalize_xy(double abs = 1)
{
	if (this->isNull())
		return v3(0, 0, 0);

	v3     result;
	double factor = abs / this->length_xy();

	result.z = this->z;
	result.x = this->x * factor;
	result.y = this->y * factor;

	return result;
}

GeoMath::v3
GeoMath::v3::normalize_xyz(double abs = 1)
{
	if (this->isNull())
		return v3(0, 0, 0);

	v3     result;
	double factor = abs / this->length_xyz();

	result.z = this->z * factor;
	result.x = this->x * factor;
	result.y = this->y * factor;

	return result;
}

GeoMath::v3
GeoMath::v3::rotateXY(double rad)
{
	v3 result(this->x * cos(rad) - this->y * sin(rad),
		this->x * sin(rad) + this->y * cos(rad),
		this->z);
	return result;
}

GeoMath::v3
GeoMath::v3::rotate(double a, GeoMath::Axis axis, Hand hand )
{
	if(hand==RIGHT) a = -a;

	v3 res(0, 0, 0);
	switch (axis)
	{
	case GeoMath::X:
		/*
		������� �������� ������ X

		| 1     0    0   |
	    | 0     cos -sin |
		| 0     sin  cos |
		*/
		res.x = this->x;
		res.y = this->y * cos(a) + this->z * sin(a);
		res.z = this->y * -sin(a) + this->z * cos(a);
		break;
	case GeoMath::Y:
		/*
		������� �������� ������ Y

		| cos   0    sin |
		| 0     1    0   |
		|-sin   0    cos |
		*/
		res.x = this->x * cos(a) - this->z * sin(a);
		res.y = this->y;
		res.z = this->x * sin(a) + this->z * cos(a);
		break;
	case GeoMath::Z:
		/*
		������� �������� ������ Z

		| cos  -sin  0   |
		| sin   cos  0   |
		| 0     0    1   |
		*/
		res.x = this->x * cos(a) + this->y * sin(a);
		res.y = this->x * -sin(a) + this->y * cos(a);
		res.z = this->z;
		break;
	}
	return res;
}


GeoMath::v2
GeoMath::v2::operator+(v2 const& v)
{
	v2 result(this->x + v.x, this->y + v.y);
	return result;
}

GeoMath::v2
GeoMath::v2::operator-(v2 const& v)
{
	v2 result(this->x - v.x, this->y - v.y);
	return result;
}

GeoMath::v2 GeoMath::v2::operator*(double factor)
{
	v2 result(*this);
	result.x = result.x * factor;
	result.y = result.y * factor;
	return result;
}

GeoMath::v2
GeoMath::v2::operator/(double demiter)
{
	v2 result(*this);
	result.x = result.x / demiter;
	result.y = result.y / demiter;
	return result;
}


GeoMath::v3
GeoMath::v3::operator+(v3 const& v2)
{
	v3 result(this->x + v2.x, this->y + v2.y, this->z + v2.z);
	return result;
}

GeoMath::v3
GeoMath::v3::operator-(v3 const& v2)
{
	v3 result(this->x - v2.x, this->y - v2.y, this->z - v2.z);
	return result;
}

GeoMath::v3 GeoMath::v3::operator*(double factor)
{
	v3 result(*this);
	result.x = result.x * factor;
	result.y = result.y * factor;
	result.z = result.z * factor;
	return result;
}

GeoMath::v3
GeoMath::v3::operator/(double demiter)
{
	v3 result(*this);
	result.x = result.x / demiter;
	result.y = result.y / demiter;
	result.z = result.z / demiter;
	return result;
}


std::ostream& GeoMath::operator<<(std::ostream& os, const GeoMath::v2& at)
{
	os << "x: " << at.x << " y: " << at.y;
	return os;
}

std::ostream& GeoMath::operator<<(std::ostream& os, const GeoMath::v3& at)
{
os <<"x: "<<at.x<<" y: "<<at.y<<" z: "<<at.z;
return os;
}



std::ostream& GeoMath::operator<<(std::ostream& os, const GeoMath::v3geo& at)
{
	os.precision(15);
	os.flags(std::ios::left | std::ios::fixed);
	os.fill('0');
	os.width(6);
	os << " lat: ";
	os.width(18);
	os<<at.lat;
	os.width(6);
	os << " lng: ";
	os.width(18);
	os << at.lng;
	os.width(6);
	os << " alt: ";
	os.width(18);
	os<<at.alt;
	return os;
}


bool
GeoMath::v3geo::isNull()
{

	return (lat == 0 && lng == 0&&alt==0);
}

GeoMath::v3
GeoMath::v3geo::operator-(v3geo const& v2)
{

	v3geo P1(v2);
	v3geo P2(*this);

	P1.lat = P1.lat * CONST.DEG2RAD;
	P1.lng = P1.lng * CONST.DEG2RAD;

	P2.lat = P2.lat * CONST.DEG2RAD;
	P2.lng = P2.lng * CONST.DEG2RAD;

	v3 P0;
	P0.x = (P2.lat - P1.lat) * CONST.P_EARTH;
	P0.y = (P2.lng - P1.lng) * CONST.E_EARTH * cos(P1.lat);
	P0.z = P2.alt - P1.alt;

	return P0;
}

GeoMath::v3geo
GeoMath::v3geo::operator+(v3 const& v2)
{

	double dlat = v2.x / CONST.M_IN_LAT;
	double m_in_lng = (cos((this->lat + dlat) * CONST.DEG2RAD) * CONST.M_IN_LNG);
	double dlng = v2.y / m_in_lng;

	v3geo result(this->lat + dlat, this->lng + dlng, this->alt + v2.z);
	return result;
}

bool GeoMath::v2::operator ==(v2 const& p2)
{
	v2 p1 = *this;
	return (p1.x == p2.x&&p1.y == p2.y);
}


bool GeoMath::v3::operator ==(v3 const& p2)
{
	v3 p1 = *this;
	return (p1.x == p2.x&&p1.y == p2.y && p1.z == p2.z);
}


bool GeoMath::v3geo::operator ==(v3geo const& p2)
{
	v3geo p1 = *this;
	return (p1.lat == p2.lng&&p1.lng == p2.lat&&p1.alt == p2.alt);
}



GeoMath::RouteLine::RouteLine()
	:points(1)
{
	points[0] = v3(0, 0, 0);
}

bool 
GeoMath::RouteLine::add_next(v3 point, PositionType position_type)
{
	switch (position_type)
	{
	case PositionType::HOME:
		points.push_back(point);
		break;

	case PositionType::OFFSET:
		points.push_back(points.back() + point);
		break;
	}
	return true;
}

GeoMath::v3 GeoMath::RouteLine::at(unsigned long i, PositionType notion)
{
	return points[i];
}

std::vector<GeoMath::v3> GeoMath::RouteLine::get_points()
{
	return std::vector<GeoMath::v3>(points);
}

bool GeoMath::RouteLine::rotate(double rad, Axis axis, Hand hand)
{
	for (std::size_t i = 0; i < points.size(); i++)
	{
		points[i]=points[i].rotate(rad, axis, hand);
	}
	return true;
}

std::vector<GeoMath::v3geo> GeoMath::RouteLine::absPosition(v3geo home)
{
	std::size_t size = points.size();
	std::vector<GeoMath::v3geo> res(size);

	for (std::size_t i = 0; i < size; i++)
	{
		res[i] = home + points[i];
	}

	return res;
}

GeoMath::RouteLine::~RouteLine()
{
}


GeoMath::RouteTemplate2D::RouteTemplate2D():
	reference_point_1_abs(0,0,0),
	reference_point_2_abs(0,0,0),
	reference_point_1_meters(0,0,0),
	reference_point_2_meters(0,0,0),
	state(GeoMath::RouteTemplate2D::NOT_SELECT),
	home_abs(0,0,0),
	home_meters(0,0),
	point_offset(1),
	point_home(1),
	scale(1),
	course(0)
{
	point_offset[0] = GeoMath::v2(0, 0);
	point_home[0] = GeoMath::v2(0, 0);
}

GeoMath::RouteTemplate2D::RouteTemplate2D(const RouteTemplate2D& route):
	reference_point_1_abs(0,0,0),
	reference_point_2_abs(0,0,0),
	reference_point_1_meters(0,0,0),
	reference_point_2_meters(0,0,0),
	state(GeoMath::RouteTemplate2D::NOT_SELECT),
	home_abs(0,0,0),
	home_meters(0,0),
	point_offset(route.point_offset),
	point_home(route.point_home),
	scale(1),
	course(0)
{
}

void GeoMath::RouteTemplate2D::add_next(PositionType position_type, v2 point)
{
	v2 home;
	v2 offset;
	switch (position_type)
	{
	case PositionType::HOME:
		
		home = point;
		offset = point_home.back() - home;
		
		break;
		
	case PositionType::OFFSET:
		
		offset = point;
		home = point_home.back() + offset;
		
		break;
	}
	
	point_home.push_back(home);
	point_offset.push_back(offset);
}

GeoMath::RouteTemplate2D::Position GeoMath::RouteTemplate2D::at(int i)
{
	Position pos;
	
	pos.home = point_home[i];
	pos.home = pos.home.rotateXY(course)*scale;
	
	pos.offset = point_offset[i];
	pos.offset = pos.offset.rotateXY(course)*scale;
	
	if (state == GeoMath::RouteTemplate2D::ABSOLUT)
		pos.abs = home_abs + pos.home;
	else
		pos.abs = v3geo(0, 0, 0);
	
	return pos;
}

GeoMath::RouteTemplate2D::Position GeoMath::RouteTemplate2D::operator[](const int i)
{
	return at(i);
}

int GeoMath::RouteTemplate2D::size()
{
	return point_home.size();
}

int GeoMath::SimpleFigure3D::size()
{
	return point_home.size();
}


GeoMath::RouteTemplate2D::~RouteTemplate2D()
{
}

bool GeoMath::RouteTemplate2D::set_reference_points(v3geo abs1, int index_1, v3geo abs2, int index_2)
{
	if (state != GeoMath::RouteTemplate2D::NOT_SELECT)
		return false;
	
	int size = this->size();
	if (index_1 >= size || index_2 >= size) 
		return false;
	
	if (abs1 == abs2)
		return false;
	
	if (index_1 == index_2)
		return false;
	
	v3 offset_abs_3d = abs2 - abs1;
	v2 offset_abs = v2(offset_abs_3d.x, offset_abs_3d.y);
	
	v2 p1 = point_home[index_1];
	v2 p2 = point_home[index_2];	
	v2 offset = p2 - p1;
	
	scale = offset_abs.length_xy() / offset.length_xy();
	course = offset_abs.angle_xy(offset);
	
	home_abs = abs1 +(p1.rotateXY(course)*scale*(-1));
	
	state = GeoMath::RouteTemplate2D::ABSOLUT;
	return true;
}

bool GeoMath::RouteTemplate2D::set_reference_points(v2 r1, int index_1, v2 r2, int index_2)
{
	if (state != GeoMath::RouteTemplate2D::NOT_SELECT)
		return false;
	
	int size = this->size();
	if (index_1 >= size || index_2 >= size) 
		return false;
	
	if (r1 == r2)
		return false;
	
	if (index_1 == index_2)
		return false;
	
	v3 offset_abs_3d = r2 - r1;
	v2 offset_abs = v2(offset_abs_3d.x, offset_abs_3d.y);
	
	v2 p1 = point_home[index_1];
	v2 p2 = point_home[index_2];	
	v2 offset = p2 - p1;
	
	scale = offset_abs.length_xy() / offset.length_xy();
	course = offset_abs.angle_xy(offset);
	
	home_meters = r1 + (p1.rotateXY(course)*scale*(-1));
	
	state = GeoMath::RouteTemplate2D::METERS;
	return true;
}

GeoMath::v2 GeoMath::RouteTemplate2D::get_home_meters()
{
	return home_meters;
}

GeoMath::SimpleFigure3D::SimpleFigure3D():
	point_home(1),
	point_offset(1)
{
	
}

void
GeoMath::SimpleFigure3D::add_next(PositionType position_type, v3 point)
{
	v3 home;
	v3 offset;
	switch (position_type)
	{
	case PositionType::HOME:
		
		home = point;
		offset = point_home.back() - home;
		
		break;
		
	case PositionType::OFFSET:
		
		offset = point;
		home = point_home.back() + offset;
		
		break;
	}
	
	point_home.push_back(home);
	point_offset.push_back(offset);
}

GeoMath::SimpleFigure3D::Position GeoMath::SimpleFigure3D::at(int i)
{
	Position pos;
	
	pos.home = point_home[i];
	pos.offset = point_offset[i];
	
	return pos;
}

GeoMath::SimpleFigure3D::Position GeoMath::SimpleFigure3D::operator[](const int i)
{
	return at(i);
}

GeoMath::SimpleFigure3D::~SimpleFigure3D()
{
}

void GeoMath::SimpleFigure3D::rotate(double rad, Axis axis, v3 from_point, Hand hand )
{
	for (std::size_t i = 0; i < size(); i++)
	{
		point_offset[i].rotate(rad, axis, from_point, hand);
		point_home[i].rotate(rad, axis, from_point, hand);
	}
}

GeoMath::v3 GeoMath::SimpleFigure3D::center()
{
	auto max_x = std::max_element(point_home.begin(), point_home.end(),[] (v3 const& l, v3 const& r) {return l.x < r.x;});
	auto max_y = std::max_element(point_home.begin(), point_home.end(),[] (v3 const& l, v3 const& r) {return l.y < r.y;});
	auto max_z = std::max_element(point_home.begin(), point_home.end(),[] (v3 const& l, v3 const& r) {return l.z < r.z;});
	
	auto min_x = std::min_element(point_home.begin(), point_home.end(), [](v3 const& l, v3 const& r) {return l.x < r.x;});
	auto min_y = std::min_element(point_home.begin(), point_home.end(), [](v3 const& l, v3 const& r) {return l.y < r.y;});
	auto min_z = std::min_element(point_home.begin(), point_home.end(), [](v3 const& l, v3 const& r) {return l.z < r.z;});
	
	double x = (*max_x + *min_x).x/2;
	double y = (*max_y + *min_y).y/2;
	double z = (*max_z + *min_z).z/2;
	
	return v3(x,y,z);
}