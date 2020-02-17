#ifndef AQUARIUM
#define AQUARIUM

#include <ctime>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <random>
#include <utility>
#include <stdexcept>

namespace aquarium {

	class Object;			// single object
	class ObjectType;		
	class Aquarium;		

	class ObjectTypeName {
		friend class ObjectType;
		friend class Aquarium;
	public:
		explicit ObjectTypeName(const std::string &_n) : value(_n) {}
	private:
		std::string value;
	};

	class X {
		friend class Location;
	public:
		explicit X(double _x) : value(_x) { if (_x < 0) throw std::runtime_error("x < 0"); }
	private:
		double value;
	};

	class Y {
		friend class Location;
	public:
		explicit Y(double _y) : value(_y) { if (_y < 0) throw std::runtime_error("y < 0"); }
	private:
		double value;
	};

	class A {
		friend class Location;
		friend class Object;
	public:
		explicit A(double _a) : value(_a) {}
	private:
		double value;
	};

	class DeathTime {
		friend class Object;
	public:
		explicit DeathTime(unsigned _dt) : value(_dt) {}
	private:
		unsigned value;
	};

	class SpeedWalk {
		friend class ObjectType;
	public:
		explicit SpeedWalk(double _sw) : value(_sw) { if (_sw < 0) throw std::runtime_error("SpeedWalk < 0"); }
	private:
		double value;
	};

	class SpeedRun {
		friend class ObjectType;
	public:
		explicit SpeedRun(double _sr) : value(_sr) { if (_sr < 0) throw std::runtime_error("SpeedRun < 0"); }
	private:
		double value;
	};

	class Vision {
		friend class ObjectType;
	public:
		explicit Vision(double _v) : value(_v) { if (_v < 0) throw std::runtime_error("Vision < 0"); }
	private:
		double value;
	};

	class BirthFrequency {
		friend class ObjectType;
	public:
		explicit BirthFrequency(unsigned _br) : value(_br) { if (_br < 0) throw std::runtime_error("BirthFrequency < 0"); }
	private:
		unsigned value;
	};

	class Lifetime {
		friend class ObjectType;
	public:
		explicit Lifetime(unsigned _lt) : value(_lt) { if (_lt < 0) throw std::runtime_error("Lifetime < 0"); }
	private:
		unsigned value;
	};

	class Settled {
		friend class ObjectType;
	public:
		explicit Settled(bool _st) : value(_st) {}
	private:
		bool value;
	};

	class MaxCount {
		friend class ObjectType;
	public:
		explicit MaxCount(unsigned _st) : value(_st) {}
	private:
		unsigned value;
	};

	class XMax {
		friend class Aquarium;
	public:
		explicit XMax(double _xm) : value(_xm) { if (_xm < 0) throw std::runtime_error("XMax < 0"); }
	private:
		double value;
	};

	class YMax {
		friend class Aquarium;
	public:
		explicit YMax(double _ym) : value(_ym) { if (_ym < 0) throw std::runtime_error("YMax < 0"); }
	private:
		double value;
	};

	class ObjectSize {
		friend class Aquarium;
	public:
		explicit ObjectSize(double _sz) : value(_sz) { if (_sz < 0) throw std::runtime_error("ObjectSize < 0"); }
	private:
		double value;
	};

	class DiplomaticStatus {
		friend class Aquarium;
	public:
		DiplomaticStatus();
		explicit DiplomaticStatus(int _ds) : value(_ds) {}
		bool operator == (const DiplomaticStatus &_f) const { return _f.value == value; }
		bool operator != (const DiplomaticStatus &_f) const { return !(_f.value == value); }
		bool operator < (const DiplomaticStatus & _ds) const { return value < _ds.value; }
		bool operator >(const DiplomaticStatus & _ds) const { return value > _ds.value; }
		DiplomaticStatus &operator ++ () { ++value; return *this; }
		DiplomaticStatus &operator -- () { --value; return *this; }
	private:
		int value = 0;
	};

	struct Location {
		Location(X _x, Y _y, A _a) : x(_x.value), y(_y.value), a(_a.value) {}
		Location(X _x, Y _y) : Location(_x, _y, A(0)) {}
		double x, y, a;
	};

	class ObjectNumber {
		friend class ObjectType;
	public:
		explicit ObjectNumber(std::vector<ObjectType>::size_type _fn) : value(_fn) { if (_fn < 0) throw std::runtime_error("ObjectNumber < 0"); }
	private:
		std::vector<ObjectType>::size_type value;
	};

	// distance between 2 locations
	double _distance(const Location _l1, const Location _l2) {
		return sqrt((_l2.x - _l1.x)*(_l2.x - _l1.x) + (_l2.y - _l1.y)*(_l2.y - _l1.y));
	}

	class Object {
	public:
		Object(Location _l, DeathTime _d, ObjectType &_ft)
			: x(_l.x), y(_l.y), a(_l.a), death_time(_d.value), Object_type(&_ft), chased(false), step_done(false) {}
		// to dodge the walls. when the fish catches up, it can swim closer to the walls
		void check_wall(bool _chase);
		void check_borders(); // so that you don’t accidentally go beyond the borders
		void stay(); // stay near its place
		void walk(); // walk and randomly turns
		// chase Object(_chase == true) or run away from it(_chase == false)
		void run(Object &_f, bool _chase);
		void eat(Object &_f);
		// sail in the indicated direction
		void control(A _a);
		// pointer to nearst Object with such diplomatic status
		Object *nearest(const DiplomaticStatus _ds, bool _is_chase) const;
		void step();
		bool is_alive() const;
		void clear_chase() { chased = false; }
		void clear_step_done() { step_done = false; }
		bool is_chased() const { return chased; }
		Location location() const { return Location(X(x), Y(y), A(a)); }
		double get_vision() const;
	private:
		double x, y, a;
		unsigned death_time;
		// is this Object chased at the moment
		bool chased;
		// is Object made step
		bool step_done;
		// pointer to ObjectType object that this Object belongs to
		ObjectType *Object_type;
	};

	class ObjectType {
		friend class Object;
	public:
		ObjectType(ObjectTypeName _ftn, SpeedWalk _sw, SpeedRun _sr, Vision _v, BirthFrequency _bf,
			Lifetime _lt, MaxCount _mc, Settled _st, Aquarium &_aq)
			: name(_ftn), speed_walk(_sw.value), speed_run(_sr.value), vision(_v.value), birth_frequency(_bf.value),
			lifetime(_lt.value), max_count(_mc.value), settled(_st.value), aquarium(&_aq) {}
		void add_Object(const Location &_l);
		// control desired fish
		void Object_control(ObjectNumber _fn, A _a);
		void delete_dead_Objectes();
		void clear_chases();
		void clear_step_done();
		std::pair<const std::string, std::vector<Location>> locations() const;
		// pointer and distance to nearst Object of this type
		std::pair<Object *, double> nearest(const Object &_f, bool _is_chase);
		void step(); // every Object make step
		std::string get_name() const { return name.value; }
		void born(); // borning of new Objectes
	private:
		ObjectTypeName name;
		std::vector<Object> Objectes; // all Objectes of this type
		// speed during walking and running
		double speed_walk, speed_run;
		double vision; //radius of vision
		unsigned birth_frequency;
		unsigned lifetime;
		// to stay ot to walk if there are no emenies in the neighbourhood
		bool settled;
		unsigned max_count; // maximum number of Object of this type
		// pointer to Aquarium object that this Object belongs to
		Aquarium *aquarium;
	};

	class Aquarium {
		friend class Object;
		friend class ObjectType;
	public:
		Aquarium(XMax _xm, YMax _ym, ObjectSize _sz)
			: x_max(_xm.value), y_max(_ym.value), Object_size(_sz.value), tempo(0),
			rand_proc(static_cast<unsigned>(std::time(nullptr))) {}
		//  all Objectes make step
		void step();
		// make this fish swim in the right direction
		void Object_control(ObjectTypeName _ftn, ObjectNumber _fn, A _a);
		// add fish type
		void add_type(ObjectTypeName _ftn, SpeedWalk _sw, SpeedRun _sr, Vision _v,
			BirthFrequency _bf, Lifetime _lt, MaxCount _mc, Settled _st);
		// add fish
		void add_Object(ObjectTypeName _ftn, const Location _l);
		// get locations of all Objectes
		std::map<std::string, std::vector<Location>> get_Object_locations() const;
		void set_diplomatic_status(const ObjectTypeName &_ftn1, const ObjectTypeName &_ftn2, const DiplomaticStatus _ds);
		unsigned current_time() const { return tempo; }
	private:
		// all Object types in aquarium.
		std::vector<ObjectType> types;
		// relations between types. more module of status, the higher the priority.
		// 0 - neutral. 1, 2, 3... - first type chases cecond one. -1, -2, -3... - first type runs away from second one.
		std::map<std::pair<const std::string, const std::string>, DiplomaticStatus> relations;
		double x_max, y_max;
		double Object_size;
		// there is a variable and function called 'time', so it is better to name this variable in Itatian
		unsigned tempo = 0;
		std::default_random_engine rand_proc;	//  for random
	};

	inline double radian_to_degree(double _angle) {
		return _angle*180.0 / 3.1415926;
	}

	inline double degree_to_radian(double _angle) {
		return _angle*3.1415926 / 180.0;
	}

	inline double Object::get_vision() const {
		return Object_type->vision;
	}

	inline bool Object::is_alive() const {
		return death_time != Object_type->aquarium->tempo;
	}

	inline void ObjectType::add_Object(const Location &_l) {
		Objectes.emplace_back(_l, DeathTime(aquarium->tempo + lifetime), *this);
	}

	inline void ObjectType::Object_control(ObjectNumber _fn, A _a) {
		Objectes[_fn.value].control(_a);
	}

	inline void Object::check_borders() {
		const double &xm = Object_type->aquarium->x_max;
		const double &ym = Object_type->aquarium->y_max;
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x > xm) x = xm;
		if (y > ym) y = ym;
	}

	inline void Object::check_wall(bool _chase) {
		const double &vis = Object_type->vision / (_chase ? 8.2 : 1.1);
		const double &xm = Object_type->aquarium->x_max;
		const double &ym = Object_type->aquarium->y_max;
		// coordinates of the unit vector with the angle a
		double i = cos(degree_to_radian(a));
		double j = sin(degree_to_radian(a));
		if (x < vis)		i += ((vis - x) / vis);
		if (y < vis)		j += ((vis - y) / vis);
		if (x > xm - vis)	i -= ((vis + x - xm) / vis);
		if (y > ym - vis)	j -= ((vis + y - ym) / vis);
		a = radian_to_degree(atan(j / i));
		if (i < 0) a += 180.0;
	}

	inline void Object::stay() {
		const double &xm = Object_type->aquarium->x_max;
		const double &ym = Object_type->aquarium->y_max;
		std::uniform_int_distribution<unsigned> unif_distr(0, 18);
		int d = unif_distr(Object_type->aquarium->rand_proc);
		switch (d) {
		case 1: x -= 0.9;	break;
		case 2: y -= 0.9;	break;
		case 3: x += 0.9;	break;
		case 4: y += 0.9;	break;
		case 5: x -= 1.8;	break;
		case 6: y -= 1.8;	break;
		case 7: x += 1.8;	break;
		case 8: y += 1.8;	break;
		}
		check_borders();
	}

	inline void Object::walk() {
		check_wall(false);
		x += (Object_type->speed_walk*cos(degree_to_radian(a)));
		y += (Object_type->speed_walk*sin(degree_to_radian(a)));
		std::uniform_int_distribution<unsigned> unif_distr(0, 15);
		int d = unif_distr(Object_type->aquarium->rand_proc);
		switch (d) {
		case 1: a += 2.1;	break;
		case 2: a -= 2.1;	break;
		case 3: a += 4.3;	break;
		case 4: a -= 4.3;	break;
		case 5: a += 7.2;	break;
		case 6: a -= 7.2;	break;
		}
		check_borders();
	}

	inline void Object::run(Object &_f, bool _chase) {
		a = radian_to_degree(atan((_f.y - y) / (_f.x - x)));
		if (!(_chase ^ (_f.x < x)))
			a += 180;
		check_wall(_chase);
		x += (Object_type->speed_run*cos(degree_to_radian(a)));
		y += (Object_type->speed_run*sin(degree_to_radian(a)));
		check_borders();
		if (_chase && _distance(this->location(), _f.location()) < Object_type->aquarium->Object_size / 2)
			eat(_f);
	}

	inline void Object::eat(Object &_f) {
		_f.death_time = Object_type->aquarium->tempo + 1;
		// food makes life longer
		death_time += Object_type->lifetime / 8;
	}

	inline void Object::control(A _a) {
		check_wall(false);
		x += (Object_type->speed_run*cos(degree_to_radian(_a.value)));
		y += (Object_type->speed_run*sin(degree_to_radian(_a.value)));
		a = _a.value;
		check_borders();
		step_done = true;
	}

	inline void ObjectType::clear_chases() {
		for (Object &one_Object : Objectes)
			one_Object.clear_chase();
	}

	inline void ObjectType::clear_step_done() {
		for (Object &one_Object : Objectes)
			one_Object.clear_step_done();
	}

	inline std::pair<const std::string, std::vector<Location>> ObjectType::locations() const {
		std::vector<Location> locs;
		for (const Object &one_Object : Objectes)
			locs.push_back(one_Object.location());
		return std::make_pair(name.value, locs);
	}

	inline void ObjectType::delete_dead_Objectes() {
		auto iter = partition(Objectes.begin(), Objectes.end(), [](const Object &_f) { return _f.is_alive(); });
		Objectes.erase(iter, Objectes.end());
	}

	inline void Aquarium::add_type(ObjectTypeName _ftn, SpeedWalk _sw, SpeedRun _sr, Vision _v,
		BirthFrequency _bf, Lifetime _lt, MaxCount _mc, Settled _st) {
		types.emplace_back(_ftn, _sw, _sr, _v, _bf, _lt, _mc, _st, *this);
		// to make this Object type neutral to all other ones
		for (const ObjectType &_ft : types) {
			set_diplomatic_status(ObjectTypeName(types.back().get_name()), ObjectTypeName(_ft.get_name()), DiplomaticStatus(0));
			set_diplomatic_status(ObjectTypeName(_ft.get_name()), ObjectTypeName(types.back().get_name()), DiplomaticStatus(0));
		}
	}

	inline void Aquarium::add_Object(ObjectTypeName _ftn, const Location _l) {
		// looking for the type with the desired name
		auto type_iter = find_if(types.begin(), types.end(),
			[&](const ObjectType &_ft){ return _ft.get_name() == _ftn.value; });
		type_iter->add_Object(_l);
	}

	inline void Aquarium::Object_control(ObjectTypeName _ftn, ObjectNumber _fn, A _a) {
		// looking for the type with the desired name
		auto type_iter = find_if(types.begin(), types.end(),
			[&](const ObjectType &_ft){ return _ft.get_name() == _ftn.value; });
		type_iter->Object_control(_fn, _a);
	}

	inline std::map<std::string, std::vector<Location>> Aquarium::get_Object_locations() const {
		std::map<std::string, std::vector<Location>> locs;
		for (const ObjectType &one_type : types)
			locs.insert(one_type.locations());
		return locs;
	}

	inline void Aquarium::set_diplomatic_status(const ObjectTypeName &_ftn1, const ObjectTypeName &_ftn2, const DiplomaticStatus _ds) {
		if (relations.find(std::make_pair(_ftn1.value, _ftn2.value)) == relations.end()) {
			relations.insert(std::make_pair(std::make_pair(_ftn1.value, _ftn2.value), _ds));
		}
		else {
			relations.at(std::make_pair(_ftn1.value, _ftn2.value)) = _ds;
		}
	}

	inline std::pair<Object *, double> ObjectType::nearest(const Object &_f, bool _is_chase) {
		// current nearest Object and distance to it
		std::pair<Object *, double> current_nearest = std::make_pair(nullptr, 1000000);
		for (Object &one_Object : Objectes) {
			double new_distance = _distance(_f.location(), one_Object.location());
			// if the new Object enters the field of view,
			// if the distance to the new processed Object is less than the distance to the nearest current,
			// if the Object, make a move, chasing Object, which still has not been pursued
			if (new_distance < _f.get_vision() && new_distance < current_nearest.second && !(_is_chase && one_Object.is_chased())) {
				current_nearest.first = &one_Object;
				current_nearest.second = new_distance;
			}
		}
		return current_nearest;
	}

	inline Object *Object::nearest(const DiplomaticStatus _ds, bool _is_chase) const {
		// current nearest Object and distance to it
		std::pair<Object *, double> current_nearest = std::make_pair(nullptr, 1000000.0);
		for (ObjectType &one_type : Object_type->aquarium->types) {
			if (Object_type->aquarium->relations.at(std::make_pair(Object_type->get_name(), one_type.get_name())) == _ds) {
				std::pair<Object *, double> new_nearest = one_type.nearest(*this, _is_chase);
				if (new_nearest.second < current_nearest.second) {
					current_nearest = new_nearest;
				}
			}
		}
		return current_nearest.first;
	}

	void Object::step() {
		// if the fish has not yet made a move
		// (she could already make a move if it is controlled by a mouse)
		// or something else
		if (!step_done) {
			DiplomaticStatus current_diplomatic_status(0);
			// looking for emenies
			// getting minimal diplomatic status
			for (const ObjectType &one_type : Object_type->aquarium->types) {
				DiplomaticStatus new_diplomatic_status(Object_type->aquarium->relations.at(std::make_pair(Object_type->get_name(), one_type.get_name())));
				if (new_diplomatic_status < current_diplomatic_status) {
					current_diplomatic_status = new_diplomatic_status;
				}
			}
			while (current_diplomatic_status != DiplomaticStatus(0)) {
				Object *nearest_Object = nearest(current_diplomatic_status, false);
				if (nearest_Object) {
					run(*nearest_Object, false); // run away
					current_diplomatic_status = DiplomaticStatus(0);
					step_done = true;
					break;
				}
				++current_diplomatic_status;
			}
			// if Object haven't made step yet, it looks for food
			if (!step_done) {
				// getting maximal diplomatic status
				for (const ObjectType &one_type : Object_type->aquarium->types) {
					DiplomaticStatus new_diplomatic_status(Object_type->aquarium->relations.at(std::make_pair(Object_type->get_name(), one_type.get_name())));
					if (new_diplomatic_status > current_diplomatic_status) {
						current_diplomatic_status = new_diplomatic_status;
					}
				}
				while (current_diplomatic_status != DiplomaticStatus(0)) {
					Object *nearest_Object = nearest(current_diplomatic_status, true);
					if (nearest_Object) {
						run(*nearest_Object, true); //  chase
						nearest_Object->chased = true;
						current_diplomatic_status = DiplomaticStatus(0);
						step_done = true;
						break;
					}
					--current_diplomatic_status;
				}
			}
			// if Object haven't found emenies or food, then it walk or stay near on it's place
			if (!step_done) {
				if (Object_type->settled) {
					stay();
				}
				else {
					walk();
				}
			}
		}
	}

	inline void ObjectType::step() {
		for (Object &one_Object : Objectes) {
			one_Object.step();
		}
	}

	inline void Aquarium::step() {
		for (ObjectType &one_type : types) {
			one_type.delete_dead_Objectes();
		}
		for (ObjectType &one_type : types) {
			one_type.step();
		}
		for (ObjectType &one_type : types) {
			one_type.clear_chases();
			one_type.clear_step_done();
		}
		for (ObjectType &one_type : types) {
			one_type.born();
		}
		++tempo;
	}

	inline void ObjectType::born() {
		auto count = Objectes.size();
		std::uniform_int_distribution<unsigned> unif_distr(0, birth_frequency - 1);
		while (count) {
			--count;
			if (Objectes.size() <= max_count && unif_distr(aquarium->rand_proc) == 1)
				add_Object(Objectes[count].location());
		}
	}

}		// namespace aquarium end

#endif