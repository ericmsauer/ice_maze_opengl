#include "level.h"

Level::Level(int level){
	this->level_num = level;
}

Level::~Level(){
	//Free Grid
	for(int i = 0; i < this->width; i++)
		free(floor_grid[i]);
	free(floor_grid);

	//Free Units
	for(int i = 0; i < this->num_units; i++)
		delete this->units[i];
	free(this->units);
}

bool Level::init_level(){
	debug_file << "BEGIN:\tInit level " << this->level_num << std::endl;
	if (!init_grid(this->level_num)){
		debug_file << "Failed to init grid" << std::endl;
		return false;
	}
	if (!init_units(this->level_num)){
		debug_file << "Failed to init units" << std::endl;
		return false;
	}
	if (!init_gates(this->level_num)){
		debug_file << "Failed to init gates" << std::endl;
		return false;
	}
	if (!init_keys(this->level_num)){
		debug_file << "Failed to init keys" << std::endl;
		return false;
	}
	debug_file << "END:\tInit Level " << this->level_num << std::endl;
	return true;
}

bool Level::init_grid(int level){
	int start_loc = 0;
	std::string line;
	
	//Load floor
	std::ifstream floor_file ("levels/level_1_floor.iml");

	if(!floor_file){
		debug_file << "Failed to open floor level file\n" << std::endl;
		floor_file.close();
		return false;
	}

	//Get starting pos
	getline(floor_file, line);
	for(size_t i = 0; i < line.size(); i++){
		if(line.at(i) == ' '){
			if(this->start_x == -1)
				this->start_x = stoi(line.substr(start_loc, i - start_loc));
			else if(this->start_y == -1)
				this->start_y = stoi(line.substr(start_loc, i - start_loc));
			else if(this->end_x == -1)
				this->end_x = stoi(line.substr(start_loc, i - start_loc));
			start_loc = i + 1;
		}
	}
	this->end_y = stoi(line.substr(start_loc, line.size() - start_loc));

	if(this->start_x == -1 || this->start_y == -1 || this->end_x == -1 || this->end_y == -1){
		debug_file << "Failed to find start and end points\n" << std::endl;
		floor_file.close();
		return false;
	}

	//Get Size
	start_loc = 0;
	getline(floor_file, line);
	for(size_t i = 0; i < line.size(); i++){
		if(line.at(i) == ' '){
			if(width == -1)
				this->width = stoi(line.substr(start_loc, i - start_loc));
			start_loc = i + 1;
		}
	}
	this->height = stoi(line.substr(start_loc, line.size() - start_loc));
	
	if(this->width == -1 || this->height == -1){
		debug_file << "Failed to find width and height" << std::endl;
		floor_file.close();
		return false;
	}

	//Init maze
	this->floor_grid = (int **) malloc(this->width * sizeof(int *));

	if(!this->floor_grid){
		debug_file << "Failed to malloc floor" << std::endl;
		floor_file.close();
		return 1;
	}

	for(int x = 0; x < this->width; x++){
		this->floor_grid[x] = (int *) malloc(this->height * sizeof(int));
		if(!this->floor_grid[x]){
			debug_file << "Failed to malloc floor[]" << std::endl;
			floor_file.close();
			return false;
		}
	}

	for(int y = this->height - 1; y >= 0; y--){
		getline(floor_file, line);
		
		int x_pos = 0;
		start_loc = 0;
		for(size_t i = 0; i < line.size(); i++){
			if(line.at(i) == ' '){
				this->floor_grid[x_pos][y] = stoi(line.substr(start_loc, i - start_loc));
				start_loc = i + 1;
				x_pos++;
			}
		}
		this->floor_grid[x_pos][y] = stoi(line.substr(start_loc, line.size() - start_loc));
	}

	return true;
}

bool Level::init_units(int level){
	std::string line;
	
	//Load units
	std::ifstream units_file ("levels/level_1_units.imu");

	if(!units_file){
		debug_file << "Failed to open units file" << std::endl;
		return false;
	}

	while(getline(units_file, line)){
		this->num_units++;
		if(!this->units)
			this->units = (Unit **) malloc(sizeof(Unit));
		else
			this->units = (Unit **) realloc(this->units, this->num_units * sizeof(Unit));

		//Did malloc/realloc fail?
		if(this->units == NULL){
			debug_file << "Failed to malloc/realloc units" << std::endl;
			units_file.close();
			return false;
		}

		int start_loc = 0;
		int id = -1;
		double *path_x = NULL, *path_y = NULL;
		int nodes = 0;

		for(size_t i = 0; i < line.size(); i++){
			if(line.at(i) == ' '){
				if (id == -1)
					id = stoi(line.substr(start_loc, i - start_loc));
				else{
					nodes++;
					double a = stod(line.substr(start_loc, i - start_loc));
					if (nodes % 2 == 1){
						if (nodes >= 3)
							path_x = (double *)realloc(path_x, ((nodes + 1) / 2) * sizeof(double));
						else
							path_x = (double *)malloc(sizeof(double));
						*(path_x + ((nodes - 1) / 2)) = a;
					}
					else{
						if (nodes >= 4)
							path_y = (double *)realloc(path_y, ((nodes + 1) / 2) * sizeof(double));
						else
							path_y = (double *)malloc(sizeof(double));
						*(path_y + ((nodes - 1) / 2)) = a;
					}
				}
				start_loc = i + 1;
			}
		}
		path_y = (double *)realloc(path_y, ((nodes + 1) / 2) * sizeof(double));
		*(path_y + nodes / 2) = stod(line.substr(start_loc, line.size() - start_loc));

		nodes = (nodes / 2) + 1;

		this->units[this->num_units - 1] = new Unit(id, path_x, path_y, nodes);
	}

	units_file.close();
	return true;
}

bool Level::init_gates(int level){
	std::string line;

	//Load units
	std::ifstream gates_file("levels/level_1_gates.imga");

	if (!gates_file){
		debug_file << "Failed to open gates file" << std::endl;
		return false;
	}

	while (getline(gates_file, line)){
		this->num_gates++;
		if (!this->gates)
			this->gates = (Gate **)malloc(sizeof(Gate));
		else
			this->gates = (Gate **)realloc(this->gates, this->num_gates * sizeof(Gate));

		//Did malloc/realloc fail?
		if (this->gates == NULL){
			debug_file << "Failed to malloc/realloc gates" << std::endl;
			gates_file.close();
			return false;
		}

		int start_loc = 0;
		int keyid = -1;
		double x_1 = -1, x_2 = -1, y_1 = -1, y_2 = -1;

		for (size_t i = 0; i < line.size(); i++){
			if (line.at(i) == ' '){
				if (keyid == -1)
					keyid = stoi(line.substr(start_loc, i - start_loc));
				else if (x_1 == -1)
					x_1 = stod(line.substr(start_loc, i - start_loc));
				else if (y_1 == -1)
					y_1 = stod(line.substr(start_loc, i - start_loc));
				else if (x_2 == -1)
					x_2 = stod(line.substr(start_loc, i - start_loc));
				start_loc = i + 1;
			}
		}
		y_2 = stod(line.substr(start_loc, line.size() - start_loc));

		this->gates[this->num_gates - 1] = new Gate(keyid, x_1, y_1, x_2, y_2);
	}

	gates_file.close();
	return true;
}

bool Level::init_keys(int level){
	std::string line;

	//Load units
	std::ifstream keys_file("levels/level_1_keys.imk");

	if (!keys_file){
		debug_file << "Failed to open keys file" << std::endl;
		return false;
	}

	while (getline(keys_file, line)){
		this->num_keys++;
		if (!this->keys)
			this->keys = (Key **)malloc(sizeof(Key));
		else
			this->keys = (Key **)realloc(this->keys, this->num_keys * sizeof(Key));

		//Did malloc/realloc fail?
		if (this->keys == NULL){
			debug_file << "Failed to malloc/realloc keys" << std::endl;
			keys_file.close();
			return false;
		}

		int start_loc = 0;
		int keyid = -1;
		double x = -1, y = -1;

		for (size_t i = 0; i < line.size(); i++){
			if (line.at(i) == ' '){
				if (keyid == -1)
					keyid = stoi(line.substr(start_loc, i - start_loc));
				else if (x == -1)
					x = stod(line.substr(start_loc, i - start_loc));
				start_loc = i + 1;
			}
		}
		y = stod(line.substr(start_loc, line.size() - start_loc));

		this->keys[this->num_keys - 1] = new Key(keyid, x, y);
	}

	keys_file.close();
	return true;
}

void Level::update(){
	for(int i = 0; i < this->num_units; i++){
		this->units[i]->update();
	}
}

void Level::draw(){
	this->draw_floor();
	this->draw_terrian();
	this->draw_grid();
	this->draw_click();

	for(int i = 0; i < this->num_units; i++){
		this->units[i]->draw();
	}
	for (int i = 0; i < this->num_gates; i++){
		this->gates[i]->draw();
	}
	for (int i = 0; i < this->num_keys; i++){
		this->keys[i]->draw();
	}
}

void Level::draw_floor(){
	glBegin(GL_QUADS);
	for(int x = 0; x < this->height; x++){
		for(int y = 0; y < this->width; y++){
			if(this->floor_grid[x][y] == 0)
				glColor3f(0.3, 0.3, 0.3);
			if(this->floor_grid[x][y] == 1)
				glColor3f(0.0, 0.4, 0.2);
			if(this->floor_grid[x][y] > 1)
				glColor3f(0.0, 0.8, 0.8);
			glVertex2f(x,y);
			glVertex2f(x+1,y);
			glVertex2f(x+1,y+1);
			glVertex2f(x,y+1);
		}
	}
	glEnd();
}

void Level::draw_grid(){
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);

    int i;
    for (i=0; i<= this->height; i++) {
        glVertex2f(0, i);
        glVertex2f(this->width, i);
    }

    for (i= 0; i<= this->width; i++) {
        glVertex2f(i, 0);
        glVertex2f(i, this->height);
    }
    glEnd();
}

void Level::draw_terrian(){

}

void Level::draw_click(){
    glPushMatrix();
    glTranslatef(this->mouse_x, this->mouse_y, 0);

    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(-.1, -.1, 0);
    glVertex3f(.1, -.1, 0);
    glVertex3f(.1, .1, 0);
    glVertex3f(-.1, .1, 0);
    glEnd();

    glPopMatrix();
}

//Collision functions
bool Level::check_unit_collision(Player *player){
	for(int i = 0; i < this->num_units; i++){
		if (this->check_player_unit_collision(player, this->units[i])){
			return true;
		}
	}
	return false;
}

bool Level::check_gate_collision(Player *player){
	for (int i = 0; i < this->num_gates; i++){
		if (!this->gates[i]->is_open()){
			if (this->check_player_gate_collision(player, this->gates[i])){
				if (player->has_item(this->gates[i]->get_keyid())){
					player->remove_item(this->gates[i]->get_keyid());
					this->gates[i]->open_gate();
				}
				else{
					return true;
				}
			}
		}
	}
	return false;
}

int Level::check_key_collision(Player *player){
	for (int i = 0; i < this->num_keys; i++){
		if (this->check_player_key_collision(player, this->keys[i])){
			this->keys[i]->pickup_key();
			return (this->keys[i]->get_id());
		}
	}
	return -1;
}

bool Level::check_player_unit_collision(Player *player, Unit *unit){
	return (this->distance(player->get_pos_x(), player->get_pos_y(), unit->get_pos_x(), unit->get_pos_y()) < player->get_radius() + unit->get_radius());
}

bool Level::check_player_key_collision(Player *player, Key *key){
	return (this->distance(player->get_pos_x(), player->get_pos_y(), key->get_x(), key->get_y()) < player->get_radius() + key->get_radius());
}

bool Level::check_player_gate_collision(Player *player, Gate *gate){
	//Closest point on collision box
	double cX, cY;

	double g_x_1 = gate->get_x_1, g_y_1 = gate->get_y_1, g_x_2 = gate->get_x_2, g_y_2 = gate->get_y_2;
	double g_thickness = gate->get_thickness();
	double *gate_points = (double *) malloc(4 * sizeof(double));
	if (g_x_1 == g_x_2){
		gate_points[0] =
		gate_points[1] =
		gate_points[2] =
		gate_points[3] =
	}
	else{
		gate_points[0] =
		gate_points[1] =
		gate_points[2] =
		gate_points[3] =
	}

	for (int Bbox = 0; Bbox < B.size(); Bbox++)
	{
		//Find closest x offset
		if (A.x < B[Bbox].x)
		{
			cX = B[Bbox].x;
		}
		else if (A.x > B[Bbox].x + B[Bbox].w)
		{
			cX = B[Bbox].x + B[Bbox].w;
		}
		else
		{
			cX = A.x;
		}

		//Find closest y offset
		if (A.y < B[Bbox].y)
		{
			cY = B[Bbox].y;
		}
		else if (A.y > B[Bbox].y + B[Bbox].h)
		{
			cY = B[Bbox].y + B[Bbox].h;
		}
		else
		{
			cY = A.y;
		}

		//If the closest point is inside the circle
		if (distance(A.x, A.y, cX, cY) < A.r)
		{
			//This box and the circle have collided
			return true;
		}
	}

	//If the shapes have not collided
	return false;
}

double Level::distance(double x_1, double y_1, double x_2, double y_2){
	return sqrt(pow(x_1 - x_2, 2) + pow(y_1 - y_2, 2));
}