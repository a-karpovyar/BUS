#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
	NewBus,
	BusesForStop,
	StopsForBus,
	AllBuses
};

struct Query {
	QueryType type;
	string bus;
	int stop_count;
	string stop;
	vector<string> stops;
};

istream& operator >> (istream& is, Query& q) {
	string operation_code;
	is >> operation_code;

	if (operation_code == "NEW_BUS") {

		q.stops.clear();
		q.type = QueryType::NewBus;
		is >> q.bus;
		is >> q.stop_count;
		for (int i = 0; i < q.stop_count; ++i) {
			string stop_1;
			is >> stop_1;
			q.stops.push_back(stop_1);
		}
	}

	else if (operation_code == "BUSES_FOR_STOP") {
		q.type = QueryType::BusesForStop;
		is >> q.stop;
	}
	else if (operation_code == "STOPS_FOR_BUS") {
		q.type = QueryType::StopsForBus;
		is >> q.bus;
	}

	else if (operation_code == "ALL_BUSES") {
		q.type = QueryType::AllBuses;
	}
	return is;
}

struct BusesForStopResponse {
	vector<string > Buses_For_Stop_Response;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
	for (const auto& bus : r.Buses_For_Stop_Response) {
		os << bus << " ";
	}
	return os;
}

struct StopsForBusResponse {
	vector <string> Stops_Response;
	vector< vector <string>> Bus_Response;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
	if (r.Stops_Response.empty()) {
		os << "No bus";
	}
	else {
		int count = 0;
		for (const auto& i : r.Stops_Response) {
			os << "Stop " << i << ": ";
			for (const auto& j : r.Bus_Response[count]) {
				os << j << " ";
			}
			if (count + 1 != r.Stops_Response.size())
				os << endl;
			++count;
		}
	}
	return os;
}

struct AllBusesResponse {
	map<string, vector<string>> All_Buses_Response;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {

	if (r.All_Buses_Response.empty()) {
		os << "No buses";
	}
	else {
		int count = 0;
		for (const auto& bus_item : r.All_Buses_Response) {
			os << "Bus " << bus_item.first << ": ";
			for (const string& stop : bus_item.second) {
				os << stop << " ";
			}

			if (count + 1 != r.All_Buses_Response.size())
				os << endl;
			++count;
		}
	}
	return os;
}

class BusManager {
private:
	map<string, vector<string>> buses_to_stops, stops_to_buses;

public:
	void AddBus(const string& bus, const vector<string>& stops) {

		buses_to_stops[bus] = stops;
		for (const auto& stop : stops) {
			stops_to_buses[stop].push_back(bus);
		}
	}

	BusesForStopResponse GetBusesForStop(const string& stop) const {
		BusesForStopResponse a;
		if (stops_to_buses.count(stop) == 0) {
			a.Buses_For_Stop_Response.push_back("No stop");
		}
		else {
			a.Buses_For_Stop_Response = stops_to_buses.at(stop);
		}
		return a;
	}

	StopsForBusResponse GetStopsForBus(const string& bus) const {
		StopsForBusResponse a;
		if (buses_to_stops.count(bus) != 0)
		{
			for (const string& stop : buses_to_stops.at(bus)) {
				a.Stops_Response.push_back(stop);
				if (stops_to_buses.at(stop).size() == 1) {
					a.Bus_Response.push_back({ "no interchange" });
				}
				else {

					vector <string> b;
					for (const string& other_bus : stops_to_buses.at(stop)) {
						if (bus != other_bus) {
							b.push_back(other_bus);
						}
					}
					a.Bus_Response.push_back(b);
				}
			}
		}

		return a;
	}

	AllBusesResponse GetAllBuses() const {
		AllBusesResponse a;
		if (!buses_to_stops.empty()) {
			for (const auto& bus_item : buses_to_stops) {
				for (const string& stop : bus_item.second) {
					a.All_Buses_Response[bus_item.first].push_back(stop);
				}
			}
		}
		return a;
	}
};


int main() {
	int query_count;
	Query q;

	cin >> query_count;

	BusManager bm;
	for (int i = 0; i < query_count; ++i) {
		cin >> q;
		switch (q.type) {
		case QueryType::NewBus:
			bm.AddBus(q.bus, q.stops);
			break;
		case QueryType::BusesForStop:
			cout << bm.GetBusesForStop(q.stop) << endl;
			break;
		case QueryType::StopsForBus:
			cout << bm.GetStopsForBus(q.bus) << endl;
			break;
		case QueryType::AllBuses:
			cout << bm.GetAllBuses() << endl;
			break;
		}
	}

	return 0;
}