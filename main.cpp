#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node{
    int ID;
    vector<Node*> next;
    
    Node(int id){
        ID = id;
        next = vector<Node*>();
    }
};

class Graph{
public:
    vector<Node*> NodeList;
    
    Graph(vector<Node*> &Nodes){
        NodeList = Nodes;
    }
    
    ~Graph(){
        for (int i = 0; i < NodeList.size(); i++){
            delete NodeList[i];
        }
    }
    
    void getDegree(){
        for (int i = 0; i < NodeList.size(); i++){
            cout << NodeList[i]->ID << " has a degree of: " << NodeList[i]->next.size() << endl;
        }
    }
    
    //NOTE ABOUT WORKING OF connectedComponent CALCULATOR:
    //1. Maintain a list of visited nodes; conveniently, the ID attribute of the the node allows easy indexing
    //2. If a node is not visited, visit it
    //3. Process visited node and its connections recursively, marking each node as visited along the way
    //4. This way, as we iterate through the NodeList, we check off multiple nodes as visited
    //5. Each time the loop in getConnectedComponents enters the if condition, it means that it is part of a new
    //   connected component.
    
    void DFShelper(int index, bool visited[]){
        visited[index] = true;
        //cout << index << " ";
        
        for (int i = 0; i < NodeList[index]->next.size(); i++){
            if (!visited[NodeList[index]->next[i]->ID]){
                DFShelper(NodeList[index]->next[i]->ID, visited);
            }
        }
    }
    
    int getConnectedComponents(){
        int result = 0;
        bool* visited = new bool[NodeList.size()];
        for (int i = 0; i < NodeList.size(); i++){
            visited[i] = false;
        }
        for (int i = 0; i < NodeList.size(); i++){
            if (visited[i] == false){
                DFShelper(i, visited);
                //cout << "\n";
                result++;
            }
        }
        return result;
    }
};

int main(){
    string line;
    int numNodes;
    vector<Node*> NodeList;
    int node1;
    int node2;
    int flag = 0;
    int id = 0;
    ifstream graphFile("graphs/n10000.txt");
    if (graphFile){
        while(getline(graphFile, line)){
            istringstream iss(line);
            if (flag == 0){
                iss >> numNodes;
                for (int i = 0; i < numNodes; i++){
                    NodeList.push_back(new Node(i));
                }
                flag++;
            }else{
                iss >> node1 >> node2;
                //This condition prevents addition of duplicate nodes to the next field of each node.
                if (find(NodeList[node1]->next.begin(), NodeList[node1]->next.end(), NodeList[node2]) == NodeList[node1]->next.end()){
                    NodeList[node1]->next.push_back(NodeList[node2]);
                }
                if (find(NodeList[node2]->next.begin(), NodeList[node2]->next.end(), NodeList[node1]) == NodeList[node2]->next.end()){
                    NodeList[node2]->next.push_back(NodeList[node1]);
                }
            }
        }
    }
    graphFile.close();
    Graph* graph = new Graph(NodeList);
//    for (int i = 0; i < numNodes; i++){
//        cout << graph->NodeList[i]->ID << " {" ;
//        if (graph->NodeList[i]->next.size() > 0){
//            for (int x = 0; x < graph->NodeList[i]->next.size() - 1; x++){
//                cout << graph->NodeList[i]->next[x]->ID << ", ";
//            }
//            cout << graph->NodeList[i]->next[graph->NodeList[i]->next.size() - 1]->ID << "}" << endl;
//        }
//    }
    graph->getDegree();
    cout << "Number of connected components: " << graph->getConnectedComponents() << endl;
    return 0;
}
