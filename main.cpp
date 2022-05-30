#include <iostream>
#include <vector>
#include <string>


using namespace std;
// ===========================================================================================
// Structures


struct room{
    // This is what will be used to hold all information about a room or area
    string name; // Name of room
    string description; // Blurb that describes the area
    vector<string> items; // Items available for picking up
    vector<string> validMove; // names of rooms that can be access from this location


    // Functions and constructor
    room(string inName, string inDesc, vector<string> inItems, vector<string> inDirections){
        name = inName;
        description = inDesc;
        items = inItems;
        validMove = inDirections;
    }

    /* Function Name: CheckForItem
     *
     * Function Description:
     * Checks Room for an available item
     *
     * Parameters:
     * item is the item that is being checked for
     *
     * return value:
     * True if room has item
     */

    bool CheckForItem(string item){
        // Checks room for an available item. Returns true if room has item
        bool hasItem = false;
        for (unsigned long long i = 0; i < items.size(); ++i) {
            if (items.at(i) == item)
                hasItem = true;
        }
        return hasItem;
    }

    /* Function Name: RemoveItem
     *
     * Function Description:
     * Seaches for item and removes it from items vector
     *
     * Parameters:
     * item is the item being removed from the items list
     */
    void RemoveItem(string item){
        // Searches for item in items vector and removes it
        // Assumes that item exists, but will not return an error if it already doesn't exist
        for (unsigned long long i = 0; i < items.size(); ++i) {
            if (items.at(i) == item){
                items.erase(items.begin() + static_cast<int>(i));
                break;
            }
        }
    }
    /* Function Name: AddItem
     *
     * Function Description:
     * This adds an item to the items vector
     *
     * Parameters:
     * item is what is added to items
     */
    void AddItem(string item){
        // Adds item to items vector
        items.push_back(item);
    }

};

class player {
    // This will hold all information about the player
public:

    player(){
        inventory = {};
        roomIndex = 0;
        currentRoom = "Kitchen";
    }
    /* Function Name: GetRoom
     *
     * Function Description:
     * Returns name of current room
     */

    string GetRoom(){
        return currentRoom;
    }

    /* Function Name: GetRoomIndex
     *
     * Function Description:
     * Returns index of current room
     */
    unsigned long long GetRoomIndex(){
        return roomIndex;
    }
    /* Function Name: InfoDump
     *
     * Function Description:
     * Used for debugging. Displays all info for player
     */

    void InfoDump(){
        // Debug info for player
        cout << "=====Info=====" << endl;
        //cout << "Health: " << health << endl;
        cout << "Inventory: ";
        for (unsigned long long i = 0; i < inventory.size(); ++i) {
            cout << inventory.at(i) << " ";
        }
        cout << endl;
        cout << "Current Room: " << currentRoom << endl;
        cout << "Room Index:   " << roomIndex << endl;
        cout << "=====XXXX=====" << endl << endl;
    }
    /* Function Name: DispInventory
     *
     * Function Description:
     * Displays inventory of player
     */
    void DispInventory(){
        if (inventory.size() == 0){
            cout << "Gabe is not currently holding anything" << endl;
        }
        else {
            cout << "Gabe currently has: ";
            for (unsigned long long i = 0; i < inventory.size()-1; ++i) {
                cout << inventory.at(i) << ", ";
            }
            cout << inventory.at(inventory.size()-1) << endl;
        }
    }
    /* Function Name: HasItem
     *
     * Function Description:
     * Checks if player has item in inventory
     *
     * Parameters:
     * item is item that is being searched for
     *
     * returns true if player has item
     */

    bool HasItem(string item){
        // Checks inventory for a given item. Returns true if includes item
        bool hasItem = false;
        for (unsigned long long i = 0; i < inventory.size(); ++i) {
            if (inventory.at(i) == item)
                hasItem = true;
        }
        return hasItem;
    }

    /* Function Name: AddItem
     *
     * Function Description:
     * adds item to inventory
     *
     * parameter:
     * item that is added to inventory
     */
    void AddItem(string item){
        // Adds item to inventory vector
        inventory.push_back(item);
    }
    /* Function Name: RemoveItem
     *
     * Function Description:
     * removes item from inventory
     *
     * parameter:
     * item that is removed from inventory
     */
    void RemoveItem(string item){
        // Searches for and removes item from inventory vector
        for (unsigned long long i = 0; i < inventory.size(); ++i) {
            if (inventory.at(i) == item){
                inventory.erase(inventory.begin() + static_cast<int>(i));
                break;
            }
        }
    }

    /* Function Name: SwitchRoom
     *
     * Function Description:
     * updates current room and current room index for player
     *
     */
    void SwitchRoom(string room, unsigned long long newIndex){
        currentRoom = room;
        roomIndex = newIndex;
    }
private:
    vector<string> inventory; // Items the player has picked up
    unsigned long long roomIndex; // Holds the index of the room
    string currentRoom; // holds name of current room
};

// ===========================================================================================
// Functions

/* Function Name: ToLower
 *
 * Function Description:
 * Converts a string to all lowercase
 */
string ToLower(string text){
    // Converts a string to all lowercase
    for (unsigned long long i = 0; i < text.length(); ++i) {
        text.at(i) = static_cast<char>(tolower(text.at(i)));
    }
    return text;
}
/* Function Name: IsValidMove
 *
 * Function Description:
 * checks if player can move to requested room from current room
 */
bool IsValidMove(vector<room> map,player user, string newRoom){
    // Checks to see if player can move to a room from their current location
    for (unsigned long long i = 0; i < map.at(user.GetRoomIndex()).validMove.size(); ++i) {
        if (ToLower(newRoom) == ToLower(map.at(user.GetRoomIndex()).validMove.at(i))){
            return true;
        }
    }
    return false;
}

bool MoveToRoom(vector<room> map, player &user, string newRoom){
    // Checks if player can move to newRoom then moves them
    // Returns true if sucessfully moved to room
    // Returns false if not a valid room
    newRoom = ToLower(newRoom);
    for (unsigned long long newRoomIndex = 0; newRoomIndex < map.size() ; ++newRoomIndex) {
        if (ToLower(map.at(newRoomIndex).name) == newRoom){
            if (IsValidMove(map,user,newRoom)){
                user.SwitchRoom(newRoom,newRoomIndex);
                return true;
            }
        }

    }
    return false;

}

bool PickUpItem(vector<room> &map, player &user, string item){
    // Removes item from room and places it in the user's inventory
    // Returns true if sucessfully picked up item
    // Returns false if item not available
    if (map.at(user.GetRoomIndex()).CheckForItem(item)){
        user.AddItem(item);
        map.at(user.GetRoomIndex()).RemoveItem(item);
        return true;
    }
    return false;
}
bool DropItem(vector<room> &map, player &user, string item){
    // Removes item from player and places it in current room
    // Returns true if sucessfully dropped item
    // Returns false if user doesn't have item
    if (user.HasItem(item)){
        user.RemoveItem(item);
        map.at(user.GetRoomIndex()).AddItem(item);
        return true;
    }
    else {
        return false;
    }
}

void LookAround(vector<room>map, player user){
    // Shows description of current room and also lists any available items
    cout << "Gabe looks around the " << map.at(user.GetRoomIndex()).name << "." << endl;
    cout << map.at(user.GetRoomIndex()).description << endl;
    if (map.at(user.GetRoomIndex()).items.size() != 0){
        cout << "Gabe can also see: ";
        for (unsigned long long i = 0; i < map.at(user.GetRoomIndex()).items.size()-1; ++i) {
            cout << map.at(user.GetRoomIndex()).items.at(i) << ", ";
        }
        cout << map.at(user.GetRoomIndex()).items.at(map.at(user.GetRoomIndex()).items.size()-1) << endl;
    }

}


void IntroScreen(){
    // Title screen with intro
    cout << endl << endl;
    cout << "       :::::::::       ::::::::       :::::::::       :::    ::: " << endl;
    cout << "      :+:    :+:     :+:    :+:      :+:    :+:      :+:   :+:   " << endl;
    cout << "     +:+    +:+     +:+    +:+      +:+    +:+      +:+  +:+     " << endl;
    cout << "    +#++:++#+      +#+    +:+      +#++:++#:       +#++:++       " << endl;
    cout << "   +#+    +#+     +#+    +#+      +#+    +#+      +#+  +#+       " << endl;
    cout << "  #+#    #+#     #+#    #+#      #+#    #+#      #+#   #+#       " << endl;
    cout << " #########       ########       ###    ###      ###    ###       " << endl;
    cout << "=================================================================" << endl;
    cout << "    ,---.     |             |    |             ,--.           " << endl;
    cout << "    |  _.,---.|---.,---.    |--- |---.,---.    |   |,---.,---." << endl;
    cout << "    |   |,---||   ||---'    |    |   ||---'    |   ||   ||   |" << endl;
    cout << "    `---'`---^`---'`---'    `---'`   '`---'    `--' `---'`---|" << endl;
    cout << "                                                         `---'" << endl;
    cout << "                           AND THE" << endl;
    cout << "    __  ___                _              ___          __   __" << endl;
    cout << "   /  |/  / ___ _  ___ _  (_) ____       / _ ) ___ _  / /  / /" << endl;
    cout << "  / /|_/ / / _ `/ / _ `/ / / / __/      / _  |/ _ `/ / /  / / " << endl;
    cout << " /_/  /_/  \\_,_/  \\_, / /_/  \\__/      /____/ \\_,_/ /_/  /_/  " << endl;
    cout << "                 /___/                                        " << endl;
    cout << endl << endl;
    cout << "                 Ready Player One!" << endl;
    cout << endl << endl << endl << endl ;

}

void HelpScreen(){
    // Displays help and commands
    cout << "Help Gabe the Dog find his magic ball!" << endl;
    cout << endl;
    cout << "Commands:"<< endl;
    cout << "help\t\t Displays this page." << endl;
    cout << "info\t\t Displays game backround." << endl;
    cout << "goto [room]\t Moves Gabe to selected room" << endl;
    cout << "get  [item]\t Picks up item from current room" << endl;
    cout << "drop [item]\t Drops item and places it in current room" << endl;
    cout << "clear\t\t Clears the screen" << endl;
    cout << "look\t\t Provides a description of the current room" << endl;
    cout << "inv\t\t Lists items in Gabe's inventory" << endl;
    cout << "map [up/down]\t Shows map of Gabe's house. (ground floor or basement)" << endl;
    cout << "open chest\t Opens chest in owner's bedroom." << endl;
    cout << "quit game\t Exits game" << endl;
    cout << endl << endl;
}

void InfoScreen(){
    // Shows background for game
    cout << endl;
    cout << "Gabe the Dog was left at home while his owner was running errands." << endl;
    cout << "Unfortunately, his owner locked up Gabe's favorite toy: the magic ball." << endl;
    cout << "Gabe saw his owner lock it in the chest in his bedroom, but needs to find the spare key in the basement." << endl;
    cout << "Help Gabe find his ball so he can 'Bork' playfully again!" << endl;
    cout << endl << endl;
}

void DispWinScreen(){
    // Winning end screen
    cout << "Congratulations! You helped Gabe find his magic ball!" << endl;
    cout << "Good boy!" << endl;
    cout << endl << endl;
    cout << "\tRest in Peace Gabe" << endl;
    cout << "\t   2011 - 2017" << endl;
    cout << endl << endl;

}

void DispTopMap(){
    // Displays upper floor
    cout << "               Ground Floor" << endl;
    cout << "+-----------------------------------------+" << endl;
    cout << "|                                         |" << endl;
    cout << "|                                         |" << endl;
    cout << "|                Back Yard                |" << endl;
    cout << "|                                         |" << endl;
    cout << "|                                         |" << endl;
    cout << "+------------+---   --------+-------------+" << endl;
    cout << "|            |              |             |" << endl;
    cout << "|  Bathroom       Dining         Kitchen  |" << endl;
    cout << "|            |     Area     |             |" << endl;
    cout << "+------------+              +------+      |" << endl;
    cout << "|   Closet   |              |Pantry       |" << endl;
    cout << "+----     ---+--          ---------+-    -+" << endl;
    cout << "|            |                     |      |" << endl;
    cout << "|   Bedroom                        |      |" << endl;
    cout << "|            |    Living Room      |Stairs|" << endl;
    cout << "+-+          |                     |      |" << endl;
    cout << "|?|          |                     |(Down)|" << endl;
    cout << "+-+----------+---------------------+------+" << endl;
    cout << endl;
}

void DispBottMap(){
    // Displays Basement
    cout << "                 Basement" << endl;
    cout << "+-------------+------------+--------------+" << endl;
    cout << "|             |            |              |" << endl;
    cout << "|   Furnace   |                           |" << endl;
    cout << "|             |                Storage    |" << endl;
    cout << "|             |            |              |" << endl;
    cout << "+-----  ------+            |              |" << endl;
    cout << "|   Hallway                +-------+------+" << endl;
    cout << "+-----  ------+                    | (Up) |" << endl;
    cout << "|             |    Dark            |      |" << endl;
    cout << "|  Laundry    |    Expanse         |Stairs|" << endl;
    cout << "|   Room      |                    |      |" << endl;
    cout << "|             |                           |" << endl;
    cout << "+-------------+--------------------+------+" << endl;
    cout << endl;
}

void GetCommand(string &verb, string &noun){
    // Has user enter a two word string in verb noun format
    // Splits into two different strings that are returned by reference
    string response;
    bool first = true;
    cout << ">>> ";
    getline(cin,response);
    for (unsigned long long letter = 0; letter < response.length(); ++letter) {
        if (first){
            if (response.at(letter) == ' '){
                first = false;
            }
            else {
                verb.push_back(response.at(letter));
            }
        }
        else {
            noun.push_back(response.at(letter));
        }
    }
}

void RoomsDebug(vector<room>map){
    cout << "===Room Debug===" << endl;
    for (unsigned long long i = 0; i < map.size(); ++i) {
        cout << "***"<< map.at(i).name << "***" << endl;
        for (unsigned long long j = 0; j < map.at(i).items.size(); ++j) {
            cout << map.at(i).items.at(j) << ", ";
        }
        cout << endl;
    }
    cout << "================" << endl;
}

void TalkToCat(player &user){
    // where gabe talks to cat
    if (ToLower(user.GetRoom()) == "back yard"){
        cout << "The cat speaks:" << endl;
        cout << "\t'Hello Gabe. Do you want this key I have? I'll trade it to you for a bag of catnip.'" << endl;
        cout << "\t'I think I saw your owner had a bag of it hidden somewhere in his room.'" << endl;
        if (user.HasItem("catnip")){
            cout << "\t'Oh that's purrrfect. Here, take the key. I was getting bored with it anyway.'" << endl;
            user.RemoveItem("catnip");
            user.AddItem("basement key");
            cout << "The cat gave Gabe the basement key!" << endl;
        }
        else {
            cout << "\t'Run along meow.'" << endl;
        }
    }
    else {
        cout << "There is nobody to talk to..." << endl;
    }

}

bool OpenChest(player user){
    // Logic for when user opens chest
    // Returns true if the user opens the chest successfully
    // Returns false if user doesn't open chest
    if (ToLower(user.GetRoom()) == "bedroom"){
        if (user.HasItem("chest key")){
            DispWinScreen();
            return true;
        }
        else {
            cout << "Gabe needs the chest key to open the chest. It is somewhere in the basement!" << endl;
            return false;
        }
    }
    else {
        cout << "The chest is in your owner's bedroom!" << endl;
        return false;
    }
}

bool ParseCommand(vector<room> &map, player &user){
    // Prompts user for command and executes it
    // Returns true if user want to quit
    // Commands: help, goto, get, drop, clear, quit, exit, look, inv, map, talk, info
    // Runs entered command and checks if it should be run again with isLeaving
    string verb, noun;
    bool isLeaving = false;
    GetCommand(verb,noun);

    noun = ToLower(noun);
    verb = ToLower(verb);

    cout << endl;

    if ( verb == "help") {
        HelpScreen();
    }
    else if (verb == "goto") {
        // Check if this is the dark expanse and if gabe has a flashlight.
        if (noun == "dark expanse" && !user.HasItem("flashlight")){
            cout << "It's too dark to see! Gabe needs a flashlight to make it through safely." << endl;
        }
        // Check if this is the stairs and if gabe has the basement key.
        else if (noun == "stairs" && !user.HasItem("basement key")) {
            cout << "The door is locked and you can't get through! Gabe needs the basement key to use the stairs." << endl;
        }
        else {
            if(MoveToRoom(map,user,noun)){
                cout << "Moved to " << noun << endl;
            }
            else {
                cout << "Gabe can't get there from the " << user.GetRoom() << endl;
            }
        }

    }
    else if (verb == "get") {
        if (PickUpItem(map,user,noun)){
            cout << "Gabe picked up the " << noun << endl;
        }
        else {
            cout << "Gabe couldn't find the " << noun << " in the " << user.GetRoom() << endl;
        }
    }
    else if (verb == "drop") {
        if (DropItem(map,user,noun)){
            cout << "Gabe dropped the " << noun <<" in the " << user.GetRoom() << endl;
        }
        else {
            cout << "Gabe couldn't find the " << noun << " in his inventory." << endl;
        }
    }
    else if (verb == "clear") {
        system("CLS");
    }
    else if (verb == "quit" && noun == "game"){
        cout << "Quitting game..." << endl;
        isLeaving = true;
    }
    else if (verb == "exit" || verb == "quit") {
        cout << "Did you mean 'quit game'?" << endl;
    }
    else if (verb == "look") {
        LookAround(map,user);
    }
    else if (verb == "inv") {
        cout << "Displaying inventory" << endl;
        user.DispInventory();
    }
    else if (verb == "map") {
        if (noun != "down"){
            DispTopMap();
    }
        else {
            DispBottMap();
        }
    }
    else if (verb == "talk") {
        TalkToCat(user);
    }
    else if (verb == "open" && noun == "chest") {
        isLeaving = OpenChest(user);
    }
    else if (verb == "info") {
        InfoScreen();
    }
    else {
        cout << "Unknown command. Please try again. For help, type 'help'." << endl;
    }
    cout << endl;
    return isLeaving;
}

void BuildRooms(vector<room> &map){
    // This builds all the rooms in the map based off of the "room" struct

    // First floor
    room Kitchen("Kitchen","There is a sink built into the counter. It often smells like food in here.",{"knife","spoon"},{"Dining Area","Pantry","Stairs"});
    map.push_back(Kitchen);
    room DiningArea("Dining Area","A large table with chairs covers most of the space.",{"fork","plate"},{"Bathroom","Back Yard","Kitchen","Living Room"});
    map.push_back(DiningArea);
    room Bathroom("Bathroom","There is a toilet with forbidden water and a sink opposite to it. There is a flashlight on a shelf.",{"flashlight"},{"Dining Area"});
    map.push_back(Bathroom);
    room LivingRoom("Living Room","The couch is forbidden for Gabe.",{"cushion"},{"Bedroom","Dining Area"});
    map.push_back(LivingRoom);
    room Bedroom("Bedroom","The bed smells like Gabe's owner. There is a locked chest in the corner.",{"pillow"},{"Closet", "Living Room"});
    map.push_back(Bedroom);
    room Closet("Closet","There are clothes hanging from the ceiling while a box is tucked into a back corner on the ground.",{"catnip"},{"Bedroom"});
    map.push_back(Closet);
    room Pantry("Pantry","There are shelves that reach the ceiling stuffed with boxes, cans, and bottles",{"human food"},{"Kitchen"});
    map.push_back(Pantry);
    room Stairs("Stairs","A set of stairs that connects the basement to the first floor.",{},{"Kitchen","Dark Expanse"});
    map.push_back(Stairs);
    room BackYard("Back Yard","A large open patch of grass is surrounded by a tall fence. There is a cat in the corner. Gabe should try talking to it.",{"tennis ball"},{"Dining Area"});
    map.push_back(BackYard);

    // Basement
    room DarkExpanse("Dark Expanse","Your flashlight cuts through the darkness like a knife, revealing a large open space.",{"dust"},{"Hallway","Storage","Stairs"});
    map.push_back(DarkExpanse);
    room Hallway("Hallway","The a red glow comes from one of the two doors off of the hallway.",{},{"Dark Expanse","Furnace","Laundry Room"});
    map.push_back(Hallway);
    room LaundryRoom("Laundry Room","There are strange smells that are coming from bottles on the shelves above the two large white boxes.",{"soap"},{"Hallway"});
    map.push_back(LaundryRoom);
    room Furnace("Furnace","The furnace is in the middle of the room. A table stands off to the side bathed in red from the furnace.",{"chest key"},{"Hallway"});
    map.push_back(Furnace);
    room Storage("Storage","There are shelves stacked with assorted items and tubs that fill the room.",{"69105 motes of dust"},{"Dark Expanse"});
    map.push_back(Storage);
}

// ===========================================================================================
// main()

int main()
{
    // Build vector of rooms
    vector<room> map;
    BuildRooms(map);

    // Build player Gabe
    player Gabe;

    // Beginning of game
    IntroScreen();
    system("PAUSE");
    InfoScreen();
    HelpScreen();


    // Testing
    bool quit = false;
    while (!quit){
        quit = ParseCommand(map, Gabe);
    }

    return 0;
}
