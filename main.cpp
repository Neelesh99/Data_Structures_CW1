/*
	First name	Neelesh
	Last name   Ravichandran
	Student ID	01334137 (e.g. 01234567)
	Username	nr1817 (e.g. aaa123)
*/

//IMPORTANT: before submitting your source code, please make sure to comment your main function (needed for automated testing)


#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <sstream>

// do not use using namespace std

// do not alter the struct declaration
struct node {
	char id;
	node* north;
	node* south;
	node* east;
	node* west;
};
// do not alter the typedef
typedef node* nodeptr;

/////////////////////////////////////////
// Functions declarations (prototypes) //
/////////////////////////////////////////

// do not alter these functions declarations

void build_basic_maze(nodeptr& start, nodeptr& finish);
// builds the basic maze displayed in page 1

nodeptr traverse_maze(const nodeptr& start, const std::string& path);
// traverses the maze using a predefined path

void solve_interactively(const nodeptr& start, const nodeptr& finish, std::string& path);
// allows the user to solve the maze interactively

nodeptr random_walk(const nodeptr& start, const nodeptr& finish, std::string& path);
// simulates a random walk in the maze

void solve_queue(const nodeptr& start, const nodeptr& finish, std::string& path);
// an improvement of the random walk: solves the maze using a queue to store the unvisited neighbours of the current node

void solve_stack(const nodeptr& start, const nodeptr& finish, std::string& path);
// an improvement of the random walk: solves the maze using a stack to store the unvisited neighbours of the current node


// you can define and add use additional functions if you might need to
bool Check_Turn(char direction, nodeptr Node);
class Map_Builder{   ///Universal Maze builder
private:
    int length;
    std::string Raw;
    char* Split;
    nodeptr start;
    nodeptr end;
public:
    Map_Builder(std::string For_Delimit):Raw(For_Delimit){ ///Constructor, using format "<StartNodeID><EndNodeID><Direction_From_Start><Node_Near_Start><Direction_From_Previous><Next_Node>...."
        length = Raw.length();
        Split = new char[length];        /// Dynamically Allocating Array Memory (De-allocated in Destructor)
        std::stringstream s_in(Raw);     /// Using String Stream Library to delimit string
        for(int i = 0;i < length;i++){
            s_in >> Split[i];            /// Storing String characters in Char Array
        }
    }
    void Build(){
        start = new node;                /// Dynamically Allocating Start Node memory
        CleanNode(start);
        start->id = Split[0];            /// Assiging Start ID
        end = new node;                  /// Dynamically Allocating End Node memory
        CleanNode(end);
        end->id = Split[1];              /// Assiging End ID
        nodeptr Prev_Node = start;       /// Setting Up Previous_Node pointer to keep track of last node
        nodeptr Curr_Node = new node;    /// Dynamically Allocating New Memory For next Node
        CleanNode(Curr_Node);
        Direction_Switch(Curr_Node,Prev_Node,Split[2]); /// Calling Direction Switch Class Method, to decide and assign the correct direction to the Map
        Curr_Node->id = Split[3];        /// Allocating Current Node to Next node in instructions
        Prev_Node = Curr_Node;           ///Storing Previous Node as Current Node
        std::stack<nodeptr> Prev_Hold;   ///Initialising Stack to store the Previous Nodes traversed, so that the '|' syntax can pull out of a particular path in a map and add nodes elsewhere
        Prev_Hold.push(start);
        int i = 4;
        while(Split[i+1] != end->id){
            if(Split[i] == '|'){         /// Implementing '|' switch to traverse the map backwards
                Prev_Node = Prev_Hold.top();
                Prev_Hold.pop();
                i++;
            }
            else {
                Curr_Node = new node;    ///Dynamically Allocating memory for new nodes
                CleanNode(Curr_Node);
                Prev_Hold.push(Prev_Node);
                Direction_Switch(Curr_Node, Prev_Node, Split[i]);
                Curr_Node->id = Split[i + 1];
                i = i + 2;
            }
        }
        Curr_Node = end;                ///Dealing with last Node
        Direction_Switch(Curr_Node,Prev_Node,Split[i]);
    }
    nodeptr GetStart(){
        return start;
    } ///Accesor for Start
    nodeptr GetFinish(){
        return end;
    } ///Accesor for Finish
    void CleanNode(nodeptr In){
        In->north = NULL;
        In->west = NULL;
        In->east = NULL;
        In->south = NULL;
    }
    void Direction_Switch(nodeptr& Curr, nodeptr& Prev, char direction){      ///Function which decides which direction to put new nodes into, aswell as pushing the Current Node into previous in preparation for next node
       switch(direction) {
           case 'n':
               Prev->north = Curr;
               Curr->south = Prev;
               Prev = Curr;
               break;
           case 's':
               Prev->south = Curr;
               Curr->north = Prev;
               Prev = Curr;
               break;
           case 'e':
               Prev->east = Curr;
               Curr->west = Prev;
               Prev = Curr;
               break;
           case 'w':
               Prev->west = Curr;
               Curr->east = Prev;
               Prev = Curr;
               break;
           default:
               return;
       }
    }
    ~Map_Builder(){    ///Deallocates the Split Array
        delete Split;
    }


};
nodeptr PathFinder(nodeptr Current,char Next);
char Reverse(char From);
struct Node_Loc{                        ///Location Class for Map Printer stores x,y co-ordinates of Given Nodes
    char id;
    int x;
    int y;
    void Inherit(Node_Loc* Parent_Loc){ ///Inherit function able to reference parent position
        x = Parent_Loc->x;
        y = Parent_Loc->y;
    }
    void Translate(char Direction){     ///Used to translate with reference to parent position
        switch (Direction) {
            case 'n':
                y++;
                break;
            case 's':
                y--;
                break;
            case 'e':
                x++;
                break;
            case 'w':
                x--;
                break;
            case 'N':
                y++;
                break;
            case 'S':
                y--;
                break;
            case 'E':
                x++;
                break;
            case 'W':
                x--;
                break;
            default:
                return;
        }
    }
};
struct COD{         ///Simulating Co-Ordinates
    int x;
    int y;
};
struct Link{        ///Used in Map-Printer to create links between Nodes
    char start;
    char end;
    char direction;
};
class Node_Frame{                                           ///Used to encapsulate and handle node information for Stack and Queue Methods
private:
    bool N;                                                 ///Booleans to indicate valid directions
    bool E;
    bool S;
    bool W;
    char ID;
    char From;                                              ///Stores the direction which led to the current node, used for tracing path
    nodeptr Node;                                           ///Current Node
    bool visited = false;                                   ///Marks the Node as visited or not
    bool Backward = false;                                  ///Flag to tell program if this node is attempting to back out of dead end
public:
    Node_Frame(nodeptr node, char from){                    ///Constructor to build Object using relevant information
        Node = node;
        ID = Node->id;
        From = from;
        //std::cout << from << "  " << From << std::endl;
        N = (Reverse(From) != 'N' && Node->north != NULL);  ///Determining of Directions are valid paths, and not the node the current node came from
        E = (Reverse(From) != 'E' && Node->east != NULL);
        S = (Reverse(From) != 'S' && Node->south != NULL);
        W = (Reverse(From) != 'W' && Node->west != NULL);
    }
    bool isDepleted(){return (!N&&!E&&!S&&!W);}             ///Determines if the Node is a dead end
    char Trace(){                                           ///Used to trace route for the path
        return From;
    }
    void Path_Back(){                                       ///Reverses Dead-End nodes, making them seem like normal node, going in the opposite direction
        From = Reverse(From);
        N = (Reverse(From) != 'N' && Node->north != NULL);
        E = (Reverse(From) != 'E' && Node->east != NULL);
        S = (Reverse(From) != 'S' && Node->south != NULL);
        W = (Reverse(From) != 'W' && Node->west != NULL);
        visited = true;
        Backward = true;
    }
    nodeptr getNode(){
        return Node;
    }
    bool Route_North(){                                     ///Tells Program whether there is valid north route and checks off the relevant direction when done
        visited = true;
        bool ret = N;
        N = false;
        return ret;
    }
    bool Route_East(){
        visited = true;
        bool ret = E;
        E = false;
        return ret;
    }
    bool Route_South(){
        visited = true;
        bool ret = S;
        S = false;
        return ret;
    }
    bool Route_West(){
        visited = true;
        bool ret = W;
        W = false;
        return ret;
    }
    bool Visited(){                                                                             ///Access Methods
        return visited;
    }
    bool isBack(){
        return Backward;
    }
};
class Map_Printer{                                                              ///Fully adaptable Map Printer Class, designed to show user the Nodes and Paths they've discovered
private:
    std::vector<std::vector<char>> Map;                                         ///Map of Nodes
    std::vector<char> ID;                                                       ///Node ID's seen so far
    std::vector<Node_Loc*> Nodes;                                               ///Node_Loc pointer store to access their information
    std::vector<Link> Linker;                                                   ///Linker vector storing the links to be drawn
    nodeptr start;
    nodeptr finish;
    Node_Loc* Starter;
    int h;
    int v;
    int max_left = 0;                                                           ///variable to store maximum extent in relevant directions for ease of drawing
    int max_right = 0;
    int max_down = 0;
    int max_up = 0;
public:
    Map_Printer(nodeptr Start,nodeptr Finish, int Horizontal, int Vertical):start(Start),finish(Finish){    ///Constructor to initialise Start and Finish Nodes as well as the desired width and height between nodes/layers
        Node_Loc* S = new Node_Loc;
        S->id = Start->id;
        S->x = 0;
        S->y = 0;
        Nodes.push_back(S);
        Starter = S;
        std::vector<char> Row0 = {S->id};
        Map.push_back(Row0);
        ID.push_back(S->id);
        h = Horizontal;
        v = Vertical;
    }
    void Next_Instruction(Node_Loc* Prev, char Direction, char ids){                                        ///Method which delas with new inputs from user
        if(!Check_ID(ids)){                                                                                 ///Checks if node has been visited before
            Node_Loc* New = new Node_Loc;                                                                   ///Generates new Node_Loc for new Node
            New->Inherit(Prev);                                                                             ///Inherits Parent location
            New->Translate(Direction);                                                                      ///Translate away from Parent Node
            New->id = ids;
            Link L;
            L.start = Prev->id;                                                                             ///Generates Link between Parent and Child Node
            L.end = New->id;
            L.direction = Direction;
            Linker.push_back(L);
            Nodes.push_back(New);
            ID.push_back(ids);
            if(Direction == 'e' || Direction == 'E'){                                                       ///Directional Statements
                if(New->x<max_right){                                                                       ///Checks if New Node, fits in the Current grid
                    Map[New->y][New->x] = New->id;
                }
                else{
                    //Map[New->y].insert(Map[New->y].end(),New->id);
                    Adjust_Map_Horizontal('E');                                                             ///If the New Node Doesn't fit, extends the grid using Adjust function
                    max_right++;
                    Map[New->y][New->x] = New->id;
                }
            }
            else if(Direction == 'w' || Direction == 'W'){
                //std::cout << "W Entered" << std::endl;
                if(New->x>=max_left){
                    //std::cout << New->x << std::endl;
                    //std::cout << Prev->x << std::endl;
                                 max_left = 0;
                    Map[New->y][New->x] = New->id;
                }
            }
            else if(Direction == 'n' || Direction == 'N'){
                if(New->y < max_up){
                    Map[New->y][New->x] = New->id;
                }
                else{
                    Adjust_Map_Vertical('N');
                    max_up++;
                    Map[New->y][New->x] = New->id;
                }
            }
            else if(Direction == 's' || Direction == 'S'){
                if(New->y >= max_down){
                    Map[New->y][New->x] = New->id;
                }
                else{
                    Adjust_Map_Vertical('S');
                    max_down = 0;
                    Map[New->y][New->x] = New->id;
                    //Starter->y++;
                }
            }
        }
        else{
			Node_Loc* Stat = new Node_Loc;			
			*Stat = *Nodes[getIndexByID(ids)];                                                                                                           ///Uses Nodes vector to track where the user is at right now
            Nodes.push_back(Stat);
        }

    }
    bool Check_ID(char search){                                                                                         ///Checks if node has been visited
        for(int i = 0; i < ID.size();i++){
            if(search == ID[i]){
                return true;
            }
        }
        return false;
    }
    void Adjust_Map_Horizontal(char Direction){                                                                         ///Adjusts Grid size based on Horizontal Extension
        switch(Direction){
            case 'E':
                for(int i = 0 ; i < Map.size();i++){
                    Map[i].push_back(' ');
                }
                break;
            case 'W':
                for(int i = 0 ; i < Map.size();i++){
                    Map[i].insert(Map[i].begin(),' ');
                }
                for(int i = 0 ; i < Nodes.size();i++){
                    Nodes[i]->x++;
                }
                break;
            default:
                std::cout << "Error" << std::endl;
        }
    }
    void Adjust_Map_Vertical(char Direction){                                                                           ///Adjusts Grid Size based in Vertical Extension
        std::vector<char> temp;
        switch(Direction){
            case 'N':
                for(int i = 0 ; i < Map[0].size();i++){
                    temp.push_back(' ');
                }
                Map.push_back(temp);
                break;
            case 'S':
                for(int i = 0 ; i < Map[0].size();i++){
                    temp.push_back(' ');
                }
                Map.insert(Map.begin(),temp);
                for(int i = 0 ; i < Nodes.size();i++){
                    Nodes[i]->y++;
                }
                break;
            default:
                std::cout << "Error" << std::endl;
        }
    }
    COD findNodeById(char Name){                                                                                        ///Allows program to find a Nodes location by its ID
        for(int i = 0; i < Map.size();i++){
            for(int j = 0; j < Map[i].size();j++){
                if(Name == Map[i][j]){
                    COD t;
                    t.y = i;
                    t.x = j;
                    return t;
                }
            }
        }
    }
    int getIndexByID(char ID){                                                                                          ///Allows Program to find a Node's index in the Nodes vector by ID
        for(int i = 0 ; i < Nodes.size();i++){
            if(Nodes[i]->id == ID){
                return i;
            }
        }
        return -1;
    }
    bool Horizontal_Link(char Start){                                                                                   ///Checks Nodes to see if there should be a Horizontal Link between them
        COD s = findNodeById(Start);
        for(int i = 0; i < Linker.size();i++){
            if(Linker[i].start == Start){
                COD e = findNodeById(Linker[i].end);
                if(e.y == s.y && e.x > s.x){
                    return true;
                }
            }
            else if(Linker[i].end == Start){
                COD e = findNodeById(Linker[i].start);
                if(e.y == s.y && e.x > s.x){
                    return true;
                }
            }
        }
        return false;
    }
    bool Vertical_Link(char Start){                                                                                     ///Checks Nodes to see if there should be a Vertical Link between
        COD s = findNodeById(Start);
        for(int i = 0; i < Linker.size();i++){
            if(Linker[i].start == Start){
                COD e = findNodeById(Linker[i].end);
                if(e.y < s.y && e.x == s.x){
                    return true;
                }
            }
            else if(Linker[i].end == Start){
                COD e = findNodeById(Linker[i].start);
                if(e.y < s.y && e.x == s.x){
                    return true;
                }
            }
        }
        return false;
    }
    void Print(){                                                                                                       ///Function Prints to Terminal
        Node_Loc* R = getCurrent();                                                                                     ///Used to locate Current Node and place a marker by it
        int Rx = R->x;
        int Ry = R->y;
        for(int i = (Map.size()-1); i >=0;i--){                                                                         ///Iterates through Grid Line by line printing relevant data
            for(int j = 0 ; j <Map[i].size();j++){
                int g = h;
                std::cout << Map[i][j];
                if(i == Ry && j == Rx){
                    std::cout << "<";                                                                                   ///Marks current Node
                    g--;
                }
                if(!Horizontal_Link(Map[i][j])) {
                    for (int k = 0; k < g; k++) {
                        std::cout << " ";
                    }
                }
                else{
                    for (int k = 0; k < g; k++) {
                        std::cout << "-";                                                                               ///Draws Horizontal Links
                    }
                }
            }
            std::cout << std::endl;
            for(int j = 0 ; j < v && (i != 0);j++){
                for(int k = 0 ; k < Map[i].size();k++){
                    if(!Vertical_Link(Map[i][k])) {
                        for(int f = 0; f < h+1;f++) {
                            std::cout << " ";
                        }
                    }
                    else{
                        std::cout << "|";                                                                               ///Draws vertical Links
                        for (int f = 0; f < h; f++) {
                            std::cout << " ";
                        }
                    }
                }
                std::cout << std::endl;
            }
        }
    }
    Node_Loc* getCurrent(){                                                                                             ///Returns Current Node
        return Nodes.back();
    }
    ~Map_Printer(){                                                                                                     ///Deallocates Dynamic Memory
        int size = Nodes.size();
        while(!Nodes.empty()){
            Node_Loc* temp;
            temp = Nodes.back();
            Nodes.pop_back();
            delete temp;
        }
    }

};
void print_options(nodeptr node);
void Reverse_Stack(std::stack<Node_Frame> &InOut);
void Queue_Recursive(std::queue<Node_Frame> &In, Node_Frame N, std::string* Path, nodeptr finish);
///////////////////
// main function //
///////////////////
int main(){
    nodeptr s = NULL;
    nodeptr e = NULL;
    build_basic_maze(s,e);
    std::string path;
    solve_interactively(s,e,path);
    std::string path2;
    std::cout << "Why" << std::endl;
    random_walk(s,e,path2);
    std::cout << path2 << std::endl;
    std::string path3;
    solve_queue(s,e,path3);
    std::cout << path3 << std::endl;
    path3 = "";
    solve_stack(s,e,path3);
    std::cout << path3 << std::endl;
    return 0;
}
//IMPORTANT (Reminder): before submitting your source code, please make sure to comment your main function (needed for automated testing)
/*int main() {

	// YOUR CODE HERE


	return 0;
}*/




/////////////////////////////////////////////
// Functions definitions (implementations) //
/////////////////////////////////////////////

// YOUR CODE HERE

void build_basic_maze(nodeptr& start, nodeptr& finish){

    ///Will use Mapper Class, with input format: "<StartNodeID><EndNodeID><Direction_From_Start><Node_Near_Start><Direction_From_Previous><Next_Node>....<Finish_Node_ID>"
    ///Will be using capitals for Node names and lower case for directions
    /// A '|' in the string instructs the Mapper to go back a node in the Map
    std::string Map = "ALsEsIeJ|||eBsFeGsK|nCeD||eHsL";  ///Required Maze
    Map_Builder m(Map);
    m.Build();
    start = m.GetStart();
    finish = m.GetFinish();
}
nodeptr traverse_maze(const nodeptr& start, const std::string& path){
    nodeptr Local = start;
    std::stringstream s_in(path); ///Using String Stream library to split the string into a string of easily accesible characters
    int length = path.length();
    char s;
    for(int i = 0; i < length; i++){
        s_in >> s;
        Local = PathFinder(Local,s);
    }
    return Local;
}
nodeptr PathFinder(nodeptr Current,char Next){  ///Simple Case based function to return a given node based on the desired direction of travel and the current node
    if(Check_Turn(Next,Current)){
        switch (Next) {
            case 'n':
                return Current->north;
            case 's':
                return Current->south;
            case 'e':
                return Current->east;
            case 'w':
                return Current->west;
            case 'N':
                return Current->north;
            case 'S':
                return Current->south;
            case 'E':
                return Current->east;
            case 'W':
                return Current->west;
            default:
                return NULL;
        }
    }
    else{
        return NULL;
    }
}
bool Check_Turn(char direction, nodeptr Node){   ///Simple Case based function to check the validity of a turn based on directional input
    switch (direction) {
        case 'n':
            return Node->north != NULL;
        case 's':
            return Node->south != NULL;
        case 'e':
            return Node->east != NULL;
        case 'w':
            return Node->west != NULL;
        case 'N':
            return Node->north != NULL;
        case 'S':
            return Node->south != NULL;
        case 'E':
            return Node->east != NULL;
        case 'W':
            return Node->west != NULL;
        default:
            return false;
    }
}
void solve_interactively(const nodeptr& start, const nodeptr& finish, std::string& path){
    std::cout << "Welcome To Interactive Solving"<< std::endl;                          ///Welcome Line
    Map_Printer MP(start,finish,5,3);                                                   ///Instantiate Map_Printer Class to provide visual map to user
    nodeptr Current;
    Current = start;
    int trys = 0;
    char Next;
    while(Current != finish) {
        std::cout << "You are in room " << start->id << " of the maze" << std::endl;    ///Simple text based feedback
        print_options(Current);                                                         ///Prints current options
        std::cout << "Here's your progress so far: " << std::endl;
        MP.Print();                                                                     ///Prints Visual representation of explored maze so far
        std::cin >> Next;
        trys++;
        if (Check_Turn(Next, Current)) {                                                ///Checks and implements turns
            switch(Next){
                case 'N':
                    Current = Current->north;
                    MP.Next_Instruction(MP.getCurrent(),Next,Current->id);
                    break;
                case 'E':
                    Current = Current->east;
                    MP.Next_Instruction(MP.getCurrent(),Next,Current->id);
                    break;
                case 'S':
                    Current = Current->south;
                    MP.Next_Instruction(MP.getCurrent(),Next,Current->id);
                    break;
                case 'W':
                    Current = Current->west;
                    MP.Next_Instruction(MP.getCurrent(),Next,Current->id);
                    break;
            }
            path+=Next;                                                                 ///Keeps track of path taken
        }
        else{
            if(Next == 'Q'){
                std::cout << "Solving Terminated" << std::endl;
                return;
            }
            else {
                std::cout << "That was an invalid operation, please try again" << std::endl;
            }
        }
    }
    MP.Print();
    std::cout << "Congratulations, you have reached the finish in " << trys << " steps" << std::endl;           ///Congratulations and number of steps
}
void print_options(nodeptr node){                   ///Simple function to print options for traversal
    std::cout << "You can go: ";
    if(Check_Turn('N',node)){
        std::cout << "(N)orth, ";
    }
    if(Check_Turn('E',node)){
        std::cout << "(E)ast, ";
    }
    if(Check_Turn('S',node)){
        std::cout << "(S)outh, ";
    }
    if(Check_Turn('W',node)){
        std::cout << "(W)est, ";
    }
    std::cout << "or (Q)uit." << std::endl;

}
nodeptr random_walk(const nodeptr& start, const nodeptr& finish, std::string& path){            ///Random Walk function
    int iteration = 0;                                                                          ///1000 iteration Limit to avoid infinite loops
    nodeptr Current = start;
    while(iteration <1000 && Current != finish){
        int available = 0;                                                                      ///Used to set max for std::rand function
        std::vector<nodeptr> Next_Options;                                                      ///Options for next node
        std::vector<char> Directions;                                                           ///For the path trace
        if(Check_Turn('N',Current)){                                                            ///Using previously defined function to determine whether we can turn that way
            available++;
            Next_Options.push_back(Current->north);
            Directions.push_back('N');
        }
        if(Check_Turn('E',Current)){
            available++;
            Next_Options.push_back(Current->east);
            Directions.push_back('E');
        }
        if(Check_Turn('S',Current)){
            available++;
            Next_Options.push_back(Current->south);
            Directions.push_back('S');
        }
        if(Check_Turn('W',Current)){
            available++;
            Next_Options.push_back(Current->west);
            Directions.push_back('W');
        }
        int index = std::rand() % available;                                                    ///Selects randomly from available paths
        Current = Next_Options[index];
        path+=Directions[index];
        iteration++;                                                                            ///Increment on iteration
    }
    return Current;
}
void solve_queue(const nodeptr& start, const nodeptr& finish, std::string& path){                           ///Function using Queues to solve maze, using recursive method
    std::queue<Node_Frame> Router;                                                                          ///Queue to store the path to end
    std::string* Path = &path;
    Node_Frame M(start,'\0');                                                                               ///Instaniation of Node_Frame Class (explained above)
    Queue_Recursive(Router,M,Path, finish);                                                                 ///Call to the recursive solver
    int Rx = Router.size();
    Router.pop();                                                                                           ///Clears Unecessary Start Node
    for(int i = 1; i < Rx;i++){
        path+=Router.front().Trace();                                                                       ///Uses Trace() member function from Node_Frame path to determine path to finish
        Router.pop();
    }
}
void Queue_Recursive(std::queue<Node_Frame> &In, Node_Frame N, std::string* Path, nodeptr finish){
    if(!N.isBack()) {                                   ///No need to re-store N if we re leaving the node the way we came
        In.push(N);
    }
    if(!N.isDepleted()){                                ///Using the isDepleted() member function to determine if a node has any more directions to visit
        if(N.Route_North()){
            Node_Frame M(N.getNode()->north,'N');
            Queue_Recursive(In,M,Path,finish);          ///Recursive call
            return;
        }
        if(N.Route_East()){
            Node_Frame M(N.getNode()->east,'E');
            Queue_Recursive(In,M,Path,finish);
            return;
        }
        if(N.Route_South()){
            Node_Frame M(N.getNode()->south,'S');
            Queue_Recursive(In,M,Path,finish);
            return;
        }
        if(N.Route_West()){
            Node_Frame M(N.getNode()->west,'W');
            Queue_Recursive(In,M,Path,finish);
            return;
        }
    }
    else if(!N.Visited()&&N.getNode() != finish){    ///If the node is depleted the we use Path_Back(), member function to determine direction to go in
        N.Path_Back();
        Queue_Recursive(In, N, Path, finish);
        return;
      }
    else{                                           ///Base case for finish node
        return;
    }
}
void solve_stack(const nodeptr& start, const nodeptr& finish, std::string& path){               ///Solve Stack implemented iteratively
    std::stack<Node_Frame> Router;                                                              ///Stack of Node_Frame objects (explained earlier)
    Node_Frame N(start,'\0');
    while(N.getNode() != finish){                                                               ///Iterative algorithm checks each node to see if it is done
        if(!N.isDepleted()){                                                                    ///If node has paths away from the direction we got to the node from
            if(!N.isBack()){                                                                    ///No need to store node we are leaving for good
                Router.push(N);
            }
            if(N.Route_North()){                                                                ///Tests to see whether we can place these on the solving stack
                Node_Frame M(N.getNode()->north,'N');
                N = M;
            }
            else if(N.Route_East()){
                Node_Frame M(N.getNode()->east,'E');
                N = M;
            }
            else if(N.Route_South()){
                Node_Frame M(N.getNode()->south,'S');
                N = M;
            }
            else if(N.Route_West()){
                Node_Frame M(N.getNode()->west,'W');
                N = M;
            }
        }
        else{
            Router.push(N);
            N.Path_Back();                                                                      ///Using Path_Back() member function to back out of dead end
        }
    }
    Router.push(N);
    Reverse_Stack(Router);                                                                      ///Stack in wrong orientation for correct printing of path
    Router.pop();                                                                               ///Getting rid off initial A node
    while(!Router.empty()){
        path+=Router.top().Trace();
        Router.pop();
    }
}
char Reverse(char From){                                    ///Simple Reverse function to find oposite direciton for traceback
    if(From == 'N'){
        return 'S';
    }
    if(From == 'S'){
        return 'N';
    }
    if(From == 'E'){
        return 'W';
    }
    if(From == 'W'){
        return 'E';
    }
    return '0';
}
void Reverse_Stack(std::stack<Node_Frame> &InOut){      ///Simple Stack reversal function
    std::stack<Node_Frame> intermediary;
    for(int i = 0; !InOut.empty();i++){
        intermediary.push(InOut.top());
        InOut.pop();
    }
    InOut = intermediary;
}


