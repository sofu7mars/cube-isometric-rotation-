#include<stdio.h>
#include<SDL2/SDL.h>
#include<math.h>

#define SURF_WIDTH 900
#define SURF_HEIGHT 600
#define COLOR_WHITE 0xffffffff
#define POINT_SHAPE 5
#define COORDINATE_OFFSET_X SURF_WIDTH/2
#define COORDINATE_OFFSET_Y SURF_HEIGHT/2
#define SHAPE_OF_CUBE 200


struct Point {
  double x;
  double y;
  double z;

};

struct Projection {
  double x;
  double y;

};

int draw_point(SDL_Surface* surface, int x, int y){

  SDL_Rect pixel = (SDL_Rect) {x, y, POINT_SHAPE, POINT_SHAPE};
  SDL_FillRect(surface, &pixel, 0xffffffff);
}

int draw_point_3d(SDL_Surface* surface, struct Point points[], int number_of_points) {

  
  for (int i = 0; i < number_of_points; i++) {
    int point_x = points[i].x + COORDINATE_OFFSET_X;
    int point_y = points[i].y + COORDINATE_OFFSET_Y;
    draw_point(surface, point_x, point_y);

  }
}

int draw_points_2d(SDL_Surface* surface, struct Projection projection[], int number_of_points) {
  for(int i = 0; i < number_of_points; i++) {

    int point_x = projection[i].x + COORDINATE_OFFSET_X;
    int point_y = projection[i].y + COORDINATE_OFFSET_Y;
    draw_point(surface, point_x, point_y);
  }

}

double rotation_for_isometric_view(struct Point points[], int number_of_points){

  double rotation_matrix[3][3] = {{sqrt(3), 0, -1 * sqrt(3)}, {1, 2, 1}, {sqrt(2), -1 * sqrt(2), sqrt(2)}};
  double result_point[3];

  for (int a = 0; a < number_of_points; a++) {
    double point_vector[3] = {points[a].x, points[a].y, points[a].z};
        for (int i = 0; i < 3; i++) {
      double dot_product = 0;
      for (int j = 0; j < 3; j++){
     
	dot_product += rotation_matrix[i][j] * point_vector[j];
      }
      result_point[i] = (1 / sqrt(6)) * dot_product;
    
    }

    points[a].x = result_point[0];
    points[a].y = result_point[1];
    points[a].z = result_point[2];
  }
  
}

double from_3d_to_2d(struct Point points[], struct Projection projection[], int number_of_points){
  double orthographic_projection[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 0}};
  double result_point[3];

  for (int a = 0; a < number_of_points; a++) {
    double point_vector[3] = {points[a].x, points[a].y, points[a].z};
    for (int i = 0; i < 3; i++) {
      double dot_product = 0;
      for (int j = 0; j < 3; j++) {
	dot_product += orthographic_projection[i][j] * point_vector[j];
      
								  
      }
      result_point[i] = dot_product;
    }

    projection[a].x = result_point[0];
    projection[a].y = result_point[1];



  }

  
  
}
int initialize_cube(struct Point points[], int number_of_points) {
 //  int size_of_points = sizeof(points) / sizeof(points[0]);
  int points_per_side = number_of_points / 12;
  
  //First line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i] = (struct Point) {-SHAPE_OF_CUBE/2 + i * step_size, SHAPE_OF_CUBE/2, +SHAPE_OF_CUBE/2};
    printf("First line points: %f, %f, %f\n", points[i].x, points[i].y, points[i].z);
  }
  //Second line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+points_per_side] = (struct Point) {-SHAPE_OF_CUBE/2 + i * step_size, -SHAPE_OF_CUBE/2, +SHAPE_OF_CUBE/2};
    printf("Second line points: %f, %f, %f\n", points[i+points_per_side].x, points[i+points_per_side].y, points[i+points_per_side].z);
  }
  //Third line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+2*points_per_side] = (struct Point) {-SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2 + i * step_size, +SHAPE_OF_CUBE/2};
  }
  //Forth line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+3*points_per_side] = (struct Point) {SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2 + i * step_size, +SHAPE_OF_CUBE/2};
  }


  //5 line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+4*points_per_side] = (struct Point) {-SHAPE_OF_CUBE/2 + i * step_size, SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2};
    //    printf("First line points: %d, %d, %d\n", points[i].x, points[i].y, points[i].z);
  }
  //6 line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+5*points_per_side] = (struct Point) {-SHAPE_OF_CUBE/2 + i * step_size, -SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2};
    //    printf("Second line points: %d, %d, %d\n", points[i+points_per_side].x, points[i+points_per_side].y, points[i+points_per_side].z);
  }
  //7 line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+6*points_per_side] = (struct Point) {-SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2 + i * step_size, -SHAPE_OF_CUBE/2};
  }
  //8 line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+7*points_per_side] = (struct Point) {SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2 + i * step_size, -SHAPE_OF_CUBE/2};
  }

  //9 line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+8*points_per_side] = (struct Point) {SHAPE_OF_CUBE/2, SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2 + i * step_size};
    //    printf("First line points: %d, %d, %d\n", points[i].x, points[i].y, points[i].z);n
  }
  //10 line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+9*points_per_side] = (struct Point) {SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2 + i * step_size};
    //    printf("Second line points: %d, %d, %d\n", points[i+points_per_side].x, points[i+points_per_side].y, points[i+points_per_side].z);
  }
  //11 line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+10*points_per_side] = (struct Point) {-SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2 + i * step_size};
  }
  //12 line
  for (int i = 0; i < points_per_side; i++){
    int step_size = SHAPE_OF_CUBE / points_per_side;
    points[i+11*points_per_side] = (struct Point) {-SHAPE_OF_CUBE/2, SHAPE_OF_CUBE/2, -SHAPE_OF_CUBE/2 + i * step_size};
  }

}




int main(int argc, char *argv[]){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("3D Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 600, 0);
  SDL_Surface* surface = SDL_GetWindowSurface(window);
  int number_of_points = 2 * SHAPE_OF_CUBE;
  struct Point point = {10, 10, 10};
  struct Point points[number_of_points];
  struct Projection projection[number_of_points];
  initialize_cube(points, number_of_points);
  rotation_for_isometric_view(points, number_of_points);
  printf("size of 3D array: %ld, size of 2D projection: %ld\n", sizeof(points), sizeof(projection));
  for (int i = 0; i < number_of_points; i++) {
    printf("x: %f, y: %f, z: %f\n", points[i].x, points[i].y, points[i].z);
  }
  SDL_Event event;
  int running = 1;
  while (running) {
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT){
	running = 0;
      }
    }
    SDL_FillRect(surface, NULL, 0x00000000);
    //draw_point_3d(surface, points, number_of_points);
    from_3d_to_2d(points, projection, number_of_points);
    draw_points_2d(surface, projection, number_of_points);
    SDL_UpdateWindowSurface(window);
    

  }
}
