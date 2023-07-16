#include <bits/stdc++.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

class Graph_M {
private:
    class Vertex {
    public:
        unordered_map<string, int> nbrs;
    };

    unordered_map<string, Vertex> vtces;

public:
    int numVertex() {
        return vtces.size();
    }

    bool containsVertex(const string& vname) {
        return vtces.count(vname) > 0;
    }

    void addVertex(const string& vname) {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void removeVertex(const string& vname) {
        Vertex vtx = vtces[vname];
        vector<string> keys;
        for (const auto& entry : vtx.nbrs) {
            keys.push_back(entry.first);
        }

        for (const string& key : keys) {
            Vertex nbrVtx = vtces[key];
            nbrVtx.nbrs.erase(vname);
        }

        vtces.erase(vname);
    }

    int numEdges() {
        int count = 0;
        for (const auto& entry : vtces) {
            count += entry.second.nbrs.size();
        }
        return count / 2;
    }

    bool containsEdge(const string& vname1, const string& vname2) {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 ||
            vtces[vname1].nbrs.count(vname2) == 0) {
            return false;
        }
        return true;
    }

    void addEdge(const string& vname1, const string& vname2, int value) {
        if (!containsEdge(vname1, vname2)) {
            vtces[vname1].nbrs[vname2] = value;
            vtces[vname2].nbrs[vname1] = value;
        }
    }

    void removeEdge(const string& vname1, const string& vname2) {
        if (containsEdge(vname1, vname2)) {
            vtces[vname1].nbrs.erase(vname2);
            vtces[vname2].nbrs.erase(vname1);
        }
    }

    void displayMap() {
        cout << "\t Delhi Metro Map" << endl;
        cout << "\t------------------" << endl;
        for (const auto& entry : vtces) {
            cout << entry.first << " =>" << endl;
            const Vertex& vtx = entry.second;
            for (const auto& nbr : vtx.nbrs) {
                cout << "\t" << nbr.first << "\t" << vtx.nbrs.at(nbr.first) << endl;
            }
        }
        cout << "\t------------------" << endl;
    }

    void displayStations() {
        cout << endl << "*************************" << endl;
        int i = 1;
        for (const auto& entry : vtces) {
            cout << i << ". " << entry.first << endl;
            i++;
        }
        cout << endl << "*************************" << endl;
    }

    bool hasPath(const string& vname1, const string& vname2, unordered_map<string, bool>& processed) {
        if (containsEdge(vname1, vname2)) {
            return true;
        }

        processed[vname1] = true;

        const Vertex& vtx = vtces[vname1];
        for (const auto& nbr : vtx.nbrs) {
            if (!processed.count(nbr.first)) {
                if (hasPath(nbr.first, vname2, processed)) {
                    return true;
                }
            }
        }

        return false;
    }

    class DijkstraPair {
public:
    string vname;
    string psf;
    int cost;

    DijkstraPair() : vname(""), psf(""), cost(0) {}

    DijkstraPair(const string& v, const string& p, int c) : vname(v), psf(p), cost(c) {}

    bool operator<(const DijkstraPair& other) const {
        return cost > other.cost;
    }
};


    int dijkstra(const string& src, const string& des, bool nan) {
        int val = 0;
        unordered_map<string, DijkstraPair> map;

        priority_queue<DijkstraPair> pq;
        for (const auto& entry : vtces) {
            const string& key = entry.first;
            DijkstraPair np(key, "", INT_MAX);

            if (key == src) {
                np.cost = 0;
                np.psf = key;
            }

            pq.push(np);
            map[key] = np;
        }

        while (!pq.empty()) {
            DijkstraPair rp = pq.top();
            pq.pop();

            if (rp.vname == des) {
                val = rp.cost;
                break;
            }

            map.erase(rp.vname);

            const Vertex& v = vtces[rp.vname];
            for (const auto& nbr : v.nbrs) {
                if (map.count(nbr.first) > 0) {
                    int oc = map[nbr.first].cost;
                    int nc;
                    if (nan) {
                        nc = rp.cost + 120 + 40 * v.nbrs.at(nbr.first);
                    } else {
                        nc = rp.cost + v.nbrs.at(nbr.first);
                    }

                    if (nc < oc) {
                        DijkstraPair gp(nbr.first, rp.psf + nbr.first, nc);
                        pq.push(gp);
                        map[nbr.first] = gp;
                    }
                }
            }
        }
        return val;
    }

    class Pair {
    public:
        string vname;
        string psf;
        int min_dis;
        int min_time;

        Pair(const string& v, const string& p, int d, int t) : vname(v), psf(p), min_dis(d), min_time(t) {}
    };

    string getMinimumDistance(const string& src, const string& dst) {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        queue<Pair> q;

        Pair sp(src, src + "  ", 0, 0);
        q.push(sp);

        while (!q.empty()) {
            Pair rp = q.front();
            q.pop();

            if (processed.count(rp.vname) > 0) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                int temp = rp.min_dis;
                if (temp < min) {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            const Vertex& rpvtx = vtces[rp.vname];
            for (const auto& nbr : rpvtx.nbrs) {
                if (processed.count(nbr.first) == 0) {
                    Pair np(nbr.first, rp.psf + nbr.first + "  ", rp.min_dis + rpvtx.nbrs.at(nbr.first), rp.min_time + 120 + 40 * rpvtx.nbrs.at(nbr.first));
                    q.push(np);
                }
            }
        }
        ans += "  -> Distance: " + to_string(min) + " meters";
        return ans;
    }
    std::vector<std::string> printCodelist()
{
    std::cout << "List of stations along with their codes:\n";

    std::vector<std::string> keys;
    for (const auto& entry : vtces)
    {
        keys.push_back(entry.first);
    }

    int i = 1, j = 0, m = 1;
    std::string temp, code;
    std::vector<std::string> codes(keys.size());
    char c;
    for (const std::string& key : keys)
    {
        std::istringstream iss(key);
        codes[i - 1] = "";
        j = 0;

        while (iss >> temp)
        {
            c = temp[0];

            while (c > 47 && c < 58)
            {
                codes[i - 1] += c;
                j++;
                c = temp[j];
            }

            if ((c < 48 || c > 57) && c < 123)
                codes[i - 1] += c;
        }

        if (codes[i - 1].length() < 2)
            codes[i - 1] += toupper(temp[1]);

        std::cout << i << ". " << key << "\t";
        if (key.length() < (22 - m))
            std::cout << "\t";
        if (key.length() < (14 - m))
            std::cout << "\t";
        if (key.length() < (6 - m))
            std::cout << "\t";
        std::cout << codes[i - 1] << std::endl;

        i++;
        if (i == pow(10, m))
            m++;
    }

    return codes;
}
void Create_Metro_Map(Graph_M g)
		{
			g.addVertex("Noida Sector 62~B");
			g.addVertex("Botanical Garden~B");
			g.addVertex("Yamuna Bank~B");
			g.addVertex("Rajiv Chowk~BY");
			g.addVertex("Vaishali~B");
			g.addVertex("Moti Nagar~B");
			g.addVertex("Janak Puri West~BO");
			g.addVertex("Dwarka Sector 21~B");
			g.addVertex("Huda City Center~Y");
			g.addVertex("Saket~Y");
			g.addVertex("Vishwavidyalaya~Y");
			g.addVertex("Chandni Chowk~Y");
			g.addVertex("New Delhi~YO");
			g.addVertex("AIIMS~Y");
			g.addVertex("Shivaji Stadium~O");
			g.addVertex("DDS Campus~O");
			g.addVertex("IGI Airport~O");
			g.addVertex("Rajouri Garden~BP");
			g.addVertex("Netaji Subhash Place~PR");
			g.addVertex("Punjabi Bagh West~P");
			
			g.addEdge("Noida Sector 62~B", "Botanical Garden~B", 8);
			g.addEdge("Botanical Garden~B", "Yamuna Bank~B", 10);
			g.addEdge("Yamuna Bank~B", "Vaishali~B", 8);
			g.addEdge("Yamuna Bank~B", "Rajiv Chowk~BY", 6);
			g.addEdge("Rajiv Chowk~BY", "Moti Nagar~B", 9);
}
};

int main() {
    Graph_M g;
    g.addVertex("Dwarka");
    g.addVertex("Janakpuri");
    g.addVertex("Palam");
    g.addVertex("IGI Airport");
    g.addVertex("Aerocity");
    g.addVertex("Vasant Vihar");
    g.addVertex("Dhaula Kuan");
    g.addVertex("Karol Bagh");
    g.addVertex("Connaught Place");
    g.addVertex("Lajpat Nagar");
    g.addVertex("Nehru Place");
    g.addVertex("Greater Kailash");

    g.addEdge("Dwarka", "Janakpuri", 6);
    g.addEdge("Dwarka", "Palam", 3);
    g.addEdge("Palam", "IGI Airport", 5);
    g.addEdge("IGI Airport", "Aerocity", 3);
    g.addEdge("IGI Airport", "Vasant Vihar", 8);
    g.addEdge("Vasant Vihar", "Dhaula Kuan", 5);
    g.addEdge("Dhaula Kuan", "Karol Bagh", 9);
    g.addEdge("Karol Bagh", "Connaught Place", 2);
    g.addEdge("Connaught Place", "Lajpat Nagar", 8);
    g.addEdge("Lajpat Nagar", "Nehru Place", 3);
    g.addEdge("Nehru Place", "Greater Kailash", 4);

   // g.displayMap();
    //g.printCodelist();
    std::cout << "\n\t\t\t**WELCOME TO THE METRO APP*" << std::endl;
    while(true){
         std::cout << "\t\t\t\t~LIST OF ACTIONS~\n\n";
        std::cout << "1. LIST ALL THE STATIONS IN THE MAP\n";
        std::cout << "2. SHOW THE METRO MAP\n";
        std::cout << "3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION AND THE PATH\n";
        std::cout << "4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        std::cout << "7. EXIT THE MENU\n";
        std::cout << "\nENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 7) : ";
        int choice;
        cin>>choice;
        if(choice==7){
            cout<<"thanks for using the app"<<endl;
            break;
        }
        string source,destination;
        int isNight,min_time;
        switch(choice){
            case 1:
            g.displayMap();
            break;
            
            case 2:
            g.printCodelist();
            break;
            
            case 4:
            cout<<"ENTER THE SOURCE STATION"<<endl;
            cin>>source;
            cout<<"ENTER THE DESTINATION STATTION"<<endl;
            cin>>destination;
            cout<<"ENTER 1 FOR DAY or 0 FOR NIGHT "<<endl;
            cin>>isNight;
            if(!g.containsVertex(source)|| !g.containsVertex(destination)){
                cout<<"Invalid source or destination station"<<endl;
            }
            min_time=g.dijkstra(source,destination,isNight);
            if(min_time!=0){
                cout << "Minimum Time: " << min_time << " minutes" << endl;
            }
            else{
                cout << "No path found between source and destination!" << endl;
            }
            break;
            case 3:
            cout<<"ENTER THE SOURCE STATION"<<endl;
            cin>>source;
            cout<<"ENTER THE DESTINATION STATTION"<<endl;
            cin>>destination;
            cout<<"ENTER THE DAY OR NIGHT "<<endl;
            cin>>isNight;
            if(!g.containsVertex(source)|| !g.containsVertex(destination)){
                cout<<"Invalid source or destination station"<<endl;
            }
            cout << "Minimum Distance: " << g.getMinimumDistance(source, destination) << endl;
            break;
        }
    }
    return 0;
}