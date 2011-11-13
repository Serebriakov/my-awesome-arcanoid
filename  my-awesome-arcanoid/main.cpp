
#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#define GLEW_STATIC
#include "GL/glew.h"

#define FREEGLUT_STATIC
#define FREEGLUT_LIB_PRAGMAS 0

#include "GL/glut.h"
#include "AL/al.h"
#include "AL/alc.h"

#endif

#include <list>
#include <stdio.h>
#include <assert.h>
#include <shader.h>
#include <file_manager.h>
#include <texture_manager.h>
#include <math.h>
#include <vector>
#include "levels.h"
#include <vector.h>
#include <sound_manager.h>

const int FRAME_RATE = 100;  


class Box {
public:

	float centre_x, centre_y, width,height;
	GLuint _texture;

	void init(float x,float y,float w,float h,GLuint texture_)
	{
		centre_x=x;
		centre_y=y;
		width=w;
		height=h;
		_texture=texture_;

	}

	void draw_box(shader &shader)
	{
		float half_width = width * 0.5f;
		float half_height = height * 0.5f;

		shader.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
				float vertices[4*4] = {
					centre_x-half_width, centre_y-half_height,0,0,
					centre_x+half_width, centre_y-half_height,1,0,
					centre_x+half_width, centre_y+half_height,1,1,
					centre_x-half_width, centre_y+half_height,0,1,
			};


	
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)vertices );
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(vertices+2) );
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(2);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		
	}

};

class brick:public Box
{
public:

	brick(int x , int y,GLuint texture_)
	{
		static const float DISTANCE_X=0.13f;
		static const float DISTANCE_Y=0.13f;
		static const float FIRST_BRICK_X=-0.7f;
		static const float FIRST_BRICK_Y=0.8f;

		centre_x=FIRST_BRICK_X + x*(DISTANCE_X);
		centre_y=FIRST_BRICK_Y -y*(DISTANCE_Y);
		width=0.1f;
		height=0.1f;
		_texture=texture_;
	}
	~brick()
	{

	}

}; 


class random {
	int seed;
public:
	random(unsigned new_seed = 0x9bac7615) : seed(new_seed) {
		seed = new_seed;
	}

	// get a floating point value
	float get(float min, float max) {
		seed = ( ( seed >> 31 ) & 0xa67b9c35 ) ^ ( seed << 1 );
		return min + ( ( seed >> 8 ) & 0xffff ) * ( ( max - min ) / 0xffff );
	}

	// get an int value
	int get(int min, int max) {
		assert(max - min <= 0xffff);
		seed = ( ( seed >> 31 ) & 0xa67b9c35 ) ^ ( seed << 1 );
		return min + ( ( seed >> 8 ) & 0xffff ) * ( max - min ) / 0xffff;
	}
};



///Our singleton class//
class Pong
{
	enum GameState
	{
		state_serving,
		state_playing,
	};

	float BALL_SPEED() { return 0.3f ;}

	 shader texture_shader_;
	//textures//
	GLuint background_texture;
	GLuint paddle_texture;
	GLuint ball_texture;
	GLuint brick_texture;
	GLuint item_texture;
	GLuint finish_texture;
	//
	GLint viewport_width_;
	GLint viewport_height_;
	std::vector<brick>wall;
	Box bat;
	Box item;
	Box ball;
	Box lvl;
	float bat_velocity_x;
	float ball_velocity_x;
	float ball_velocity_y;
	GameState game_state;
	char key_states[256];
	Levels thisLevel;
	int score;    // keeps track of the smashed bricks			
	bool item_drop;  // if its true an item drops in the screen
	bool took_item; //if its true it stops spawning items for the current level
	random randomness;
	ALuint buffers[256];

	void draw_world(shader &shader)

	{
		lvl.draw_box(texture_shader_);
		bat.draw_box(texture_shader_);
		ball.draw_box(texture_shader_);

		for (unsigned int i=0;i<wall.size();i++)
		{
			if (thisLevel.current_level[i]==1)
				wall[i].draw_box(texture_shader_);
		}		

		if (item_drop==true)
		{
			item.draw_box(texture_shader_);
		}
	}


	void render()
	{
		glClearColor(0, 0 ,0, 0);
		glViewport(0, 0, viewport_width_, viewport_height_);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		draw_world(texture_shader_);

		drawString(-0.9f,0.9f,0);


		glutSwapBuffers();
	}

	void sound() {
		int sound = randomness.get(1, 7);
		char name[2];
		name[0] = sound + '0';
		name[1] = 0;
		sound_manager::play(vec4(0, 0, 0, 0), name);
	}

	

	Pong ()
	{
	sound_manager::add_buffers("assets/labels.txt", "assets/poing.wav");

	imageloader();  // converts our tga files into the textures we'll use 

	lvl.init(0,0,2.0f,2.0f,background_texture);
	bat.init(0,-0.8f,0.5f,0.06f,paddle_texture);
	ball.init(0,0,0.06f,0.06f,ball_texture);
	item.init(0,0.4f,0.1f,0.1f,item_texture);


		
		bat_velocity_x=0.05f; 
	
		ball_velocity_x=BALL_SPEED();
		ball_velocity_y=BALL_SPEED();
		game_state=state_serving; 
		score=0;   
		item_drop=false;  
		took_item=false;

		thisLevel.change(1);
	
		

		texture_shader_.init
			(
			"varying vec2 uv_;"
			"attribute vec4 pos;"
			"attribute vec2 uv;"
			"void main() { gl_Position = pos; uv_ = uv; }",

			"varying vec2 uv_;"
			"uniform sampler2D texture;"
			"void main() { gl_FragColor = texture2D(texture, uv_); }"
			);

		
		for (int i=0;i<10;i++)			
		{	
			for (int j=0;j<12;j++)
			{
			wall.push_back(brick(j,i,brick_texture));
			}
		} 	

	
	};


	void imageloader()  // this is called once to save all the textures we will use
	{	
	background_texture=texture_manager::new_texture("assets/marvel.tga", 0, 0, 960, 600);
	paddle_texture=texture_manager::new_texture("assets/bat.tga", 0, 0, 200, 60); 
	ball_texture=texture_manager::new_texture("assets/ball.tga", 0, 0, 30, 30);
	brick_texture=texture_manager::new_texture("assets/brick.tga", 0, 0, 80, 156);
	item_texture=texture_manager::new_texture("assets/slow.tga", 0, 0, 104, 80);
	finish_texture=texture_manager::new_texture("assets/buddy.tga", 0, 0, 788, 600);

	}




	void level_update()		
{
	


	int sum=0;
	for (int i=0;i<120;i++)
		{	
		sum+=thisLevel.current_level[i];
		} // adds the integer value of all the elements in the current_level array

	if (sum==0)  // the level is over/no more bricks!
{
		took_item=false; // we reset the state of the item,so it drops again on next level
		ball_velocity_x=ball_velocity_x*0.8f;  // we slow down a bit so we don't reach ridiculous speed
		ball_velocity_y=ball_velocity_y*0.8f;
	

	if (thisLevel.lvl==1)   
		{	
		background_texture=texture_manager::new_texture("assets/banksy.tga", 0, 0, 700, 525);
		lvl.init(0,0,2.0f,2.0f,background_texture);
		thisLevel.change(2); 
		}

	else if (thisLevel.lvl==2)
		{
		background_texture=texture_manager::new_texture("assets/malleus.tga", 0, 0, 256, 256);
		lvl.init(0,0,2.0f,2.0f,background_texture);
		thisLevel.change(3); 
		}

	else if (thisLevel.lvl==3)
		{
		
		game_state=state_serving;
		lvl.init(0,0,2.0f,2.0f,finish_texture);
		lvl.draw_box(texture_shader_);
		}

}

		
}

	bool collides(Box *box_a, Box *box_b)
	{
		float gap_x = (box_a->width + box_b->width) * 0.5f;
		float gap_y = (box_a->height + box_b->height) * 0.5f;
		return
			(
			fabsf(box_a->centre_x - box_b->centre_x) <= gap_x &&
			fabsf(box_a->centre_y - box_b->centre_y) <= gap_y
			);
	}


	void angle_correction()
	{	


		if(ball_velocity_y/ball_velocity_x>=2.0f)  // if the ball_velocity angle gets too steep we correct is
		{
			float a=1.3f;   
    		float b = sqrt ( ( ( (ball_velocity_x*ball_velocity_x)*(1-(a*a)) ) / (ball_velocity_y*ball_velocity_y) ) + 1 ); //calculates the b variable so we maintain the same speed 
																														//(note : could use sin and cosin for this as well)
 
		}
		else if(ball_velocity_y/ball_velocity_x<2.0f)
		{
			float a=1.0f;
			float b = 1.0f;
		}


		if(ball_velocity_x/ball_velocity_y>=2.0f)  // if the ball_velocity angle gets too wide
		{
			float a=0.7f;   
    		float b = sqrt ( ( ( (ball_velocity_x*ball_velocity_x)*(1-(a*a)) ) / (ball_velocity_y*ball_velocity_y) ) + 1 );
		}
		else if(ball_velocity_x/ball_velocity_y<2.0f)
		{
			float a = 1.0f;
			float b = 1.0f;
		}

	}



// All the gameplay. Ball and bat movement, collision detection, game state, item drop, level change
	void animate()
	{	
		float a=1.0f;  //angle correction variables
		float b=1.0f;
		angle_correction(); 

		level_update();	//checks if a level is finished

						
		
		
		if (key_states['d'] && bat.centre_x < 0.7f)
		{
			bat.centre_x += bat_velocity_x;
		}
		if (key_states['a'] && bat.centre_x > -0.7f)
		{
			bat.centre_x -= bat_velocity_x;
		}


		if(game_state == state_serving)
		{
			ball.centre_y = bat.centre_y + 0.1f;
			ball.centre_x = bat.centre_x;

			if (key_states[' '] != 0)
			{
				game_state = state_playing;
				ball_velocity_x = 1;
				ball_velocity_y = 1;
			}

		} 

		else // if ball is in play
		{
			ball.centre_y += ball_velocity_y * (1.0f / FRAME_RATE);
			ball.centre_x += ball_velocity_x * (1.0f / FRAME_RATE);

			if (ball.centre_y < -1.0f)// if ball misses bat and falls 
			{
				game_state = state_serving;
				score=score-30;
				item_drop=false; // in case we lose while an item is falling
			}

			//WALL COLLISION//
			if (ball.centre_x >= 0.90f || ball.centre_x <= -0.90f)  //if ball hits right or left wall
			{	
				ball_velocity_x = -ball_velocity_x * a ; // angle correction
				ball_velocity_y=ball_velocity_y * b;
			}
			
			if (ball.centre_x >= 0.91f || ball.centre_x <= -0.91f)
				game_state = state_serving;            // temporary bug fix



			if (ball.centre_y > 0.9f)  //if ball hits ceiling
			{
				angle_correction();
				ball_velocity_y = -ball_velocity_y * (1.05f *b); //ball speeds up a bit 
				ball_velocity_x = ball_velocity_x *a;

			} 

			//COLLISION WITH THE PADDLE//
			if (collides(&ball,&bat))
			{
		

			float fabsf_gap_x=fabsf(ball.centre_x-bat.centre_x); // gap between ball and paddle on the x-axis
			float gap_x=(ball.centre_x-bat.centre_x);  
		

			if  (fabsf_gap_x>=(bat.width*0.25f))  // if the ball hits the paddle on the left or right quarters

			{
				if(gap_x<0 && ball_velocity_x>0)  //if it hits left side coming from the left
				{
					ball_velocity_x=   -ball_velocity_x * a; 
					ball_velocity_y=   -ball_velocity_y * b; 
			
				}

				else if(gap_x<0 && ball_velocity_x<0) //if it hits left side coming from the right
				{	ball_velocity_x= ball_velocity_x * a;
					ball_velocity_y=-ball_velocity_y * b; 
				}

				if (gap_x>0 && ball_velocity_x<0)  //if it hits right side coming from the right
				{
					ball_velocity_x=   -ball_velocity_x * a; 
					ball_velocity_y=   -ball_velocity_y * b; 

				}

				else if (gap_x>0 && ball_velocity_x>0) //if it hits right side coming from the left
				{	
					ball_velocity_y=   -ball_velocity_y * b; 
				}
			
			}
			else
			ball_velocity_y = -ball_velocity_y * b ;
		
			
			}
			
			//COLLISION WITH BRICKS//
			for (unsigned int i=0;i<wall.size();i++)
			{	
					if (thisLevel.current_level[i]==1 && collides(&ball, &wall[i])) // if the state of the brick==1 the brick is drawn
					{							
						ball_velocity_x = -ball_velocity_x;
						sound();

						thisLevel.current_level[i]=0;  
						score=score+1;	
						
					}					
			}
				//ITEM DROP//
				if (collides(&ball, &wall[5])&& took_item==false) // we check if the item has spawned before
				{
					item_drop=true; 
				}


				if(item_drop==true)
				{
					item.centre_y-=0.004f; 
				}
							

				if (collides(&item, &bat)&& item_drop==true) 
				
				{	
				item_drop=false;
				took_item=true;
				item.centre_y=0; //we reset the item's position
				ball_velocity_x = ball_velocity_x*0.7f;
				ball_velocity_y = ball_velocity_y*0.7f;		
				}
		}

	}



	// when the window size changes, adjust the viewport
	
	
	void set_viewport(int w, int h)
	{
			viewport_width_ = w;
			viewport_height_ = h;
	}



	void drawString(float x, float y, float z) // draws score on screen
	{ 

		glEnable(GL_COLOR_MATERIAL);
		glColor3f(0.8f, 0.2f, 0.0f);
		glRasterPos3f(x, y, z);
		char scoreArr[300];
		sprintf_s(scoreArr,"Bricks Smashed :" "%d", score );

		for (const char* c=scoreArr; *c != '\0'; c++)
		{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); 
		}
		

	}

	// change the game state and draw
	void pong_display(void)
	{

		animate();

		render();

	}

	// when a key goes down, set the key state
	void keyboard(unsigned char key, int x, int y)
	{
		key_states[key] = 1;
		switch (key) {
case 27: //Escape key
	exit(0); //Exit the program
		}
	}

	// when a key goes up, reset the key state
	void keyboardUp(unsigned char key, int x, int y)
	{
		key_states[key] = 0;
	}




public:
	// a singleton: one instance of this class only!
	static Pong &get()
	{
		static Pong singleton;
		return singleton;
	}

	static void display() { get().pong_display(); }
	static void reshape(int w, int h) { get().set_viewport(w, h); }
	static void key_down( unsigned char key, int x, int y) { get().keyboard(key, 1,0); }
	static void key_up( unsigned char key, int x, int y) { get().keyboardUp(key, 0,0); }
	static void timer(int value) { glutTimerFunc(1000/FRAME_RATE, timer, 1); glutPostRedisplay(); }

};

// set up glut callback functions

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(650, 700);
	glutCreateWindow("Brick Smasher");
#ifdef WIN32
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0") )
	{
		printf("OpenGL 2 is required!\n");
		return 1;
	}
#endif
	glutDisplayFunc(Pong::display);
	glutReshapeFunc(Pong::reshape);
	glutTimerFunc(1000/FRAME_RATE,&Pong::timer,1);
	glutKeyboardFunc(Pong::key_down);
	glutKeyboardUpFunc(Pong::key_up);
	glutMainLoop();
	return 0;
}


