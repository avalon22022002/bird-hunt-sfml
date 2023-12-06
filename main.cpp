#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <climits>
#include <iostream>
#include<vector>
#include <utility>
#include<string>
#include <cmath>
#include<random>

using namespace std;


int getRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_int_distribution<int> dist;

    return dist(mt, decltype(dist)::param_type{min, max});
}

class WindowManager {
public:
    static sf::RenderWindow& getWindow() {
        static sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
        return window;
    }
};

class Scope{
    private:
    sf::Texture scope_texture;
    sf::Sprite scope_sprite;
    sf::SoundBuffer sound_buffer;
    sf::Sound sound;

    long int bullets=INT_MAX;
    public:

    void initialize(){
        this->scope_texture.loadFromFile("./assets/sniper_Scope.png");
        this->scope_texture.setSmooth(true);
        this->scope_sprite.setTexture(this->scope_texture);
    }

    Scope(){
         this->initialize();
    }

    void setPosition(float x, float y){
        auto h=this->scope_sprite.getLocalBounds();
        this->scope_sprite.setOrigin(h.width/2,h.height/2);
        auto oc=this->scope_sprite.getOrigin();
        this->scope_sprite.setPosition(x-oc.x/10,y-oc.y/10);
    }

    void draw(){
        WindowManager::getWindow().draw(this->scope_sprite);
    }
    
    void play_sound(){
        this->sound_buffer.loadFromFile("./audio/gun_shot.wav");
        this->sound.setBuffer(this->sound_buffer);
        this->sound.play();
    }

    long int get_bullets_remaining(){
        return this->bullets;
    }

};

class Bird{
    private:
        sf::Texture bird_texture;
        sf::Sprite bird_sprite;
        sf::Clock clock ,speed_clock, sound_clock;
        sf::SoundBuffer sound_buffer;
        sf::Sound sound;
        string frames_folder_path;
        int number_of_frames=0;
        int imageno=0;
        float frame_rate_in_seconds=0;
        float x1,y1,x2,y2,speed,x,y;
        bool is_falling=false,fall_start=false;

    public:
       
        Bird(){
            this->number_of_frames=15;
            this->frame_rate_in_seconds=20;
            this->frames_folder_path="./assets/bird/bird_1/";
            this->sound_buffer.loadFromFile("./audio/bird_sound.wav");
            this->sound.setBuffer(this->sound_buffer);
            this->initialize();
        }

        Bird(int number_of_frames,float frame_rate_in_seconds, string frames_folder_path){
            this->frames_folder_path=frames_folder_path;
            this->number_of_frames=number_of_frames;
            this->frame_rate_in_seconds=frame_rate_in_seconds;
            this->initialize();

        }

        void initialize(){
            this->clock.restart();
            this->speed_clock.restart();
            this->x1=0;this->y1=getRandomNumber(1,WindowManager::getWindow().getSize().y);
            this->x2=WindowManager::getWindow().getSize().x;this->y2=getRandomNumber(1,WindowManager::getWindow().getSize().y);
            this->x=this->x1;this->y=this->y1;
            this->speed=getRandomNumber(900,1200);
            double m=(this->y2-this->y1)/(this->x2-this->x1);
            double angleInDegrees = atan(m)*(180.0 / M_PI);  //atan() == taninverse(); , M_PI=3.142
            this->bird_sprite.setRotation(angleInDegrees);
            this->bird_sprite.setPosition(this->x,this->y);
        }

        void set_frames_folder_path(string frames_folder_path){
            this->frames_folder_path=frames_folder_path;
        }
        
        void set_frame_rate_in_seconds(float frame_rate_in_seconds){
            this->frame_rate_in_seconds=frame_rate_in_seconds;
        }

        void set_number_of_frames(int number_of_frames){
            this->number_of_frames=number_of_frames;
        }


        void update_bird_frame(){
            if(this->clock.getElapsedTime().asMilliseconds()>=(1000/this->frame_rate_in_seconds)) {
                this->imageno=(this->imageno+1)%this->number_of_frames;
                this->clock.restart();
            }
            if(this->is_falling) {
                this->bird_texture.loadFromFile(this->frames_folder_path+"dead.png");
                auto textureSize=this->bird_texture.getSize();
                this->bird_texture.setSmooth(true);
                this->bird_sprite.setTexture(this->bird_texture);
                auto h=this->bird_sprite.getLocalBounds();
                this->bird_sprite.setOrigin(h.width/2,h.height/2);
            }else{
                this->bird_texture.loadFromFile(this->frames_folder_path+to_string(this->imageno)+".png");
                auto textureSize=this->bird_texture.getSize();
                this->bird_texture.setSmooth(true);
                this->bird_sprite.setTexture(this->bird_texture);
                auto h=this->bird_sprite.getLocalBounds();
                this->bird_sprite.setOrigin(h.width/2,h.height/2);
            }
            
        }

        void update_along_line(){
            this->update_bird_frame();
            if(this->speed_clock.getElapsedTime().asMilliseconds()>=(1000/this->speed)) { 
                if(this->x2==this->x1){
                    this->y++;
                    //vertical line case
                }
                else {
                    this->x++;
                    double mp=(this->y2-this->y1)/(this->x2-this->x1);
                    y=mp*(this->x-this->x1)+y1;
                }
                this->bird_sprite.setPosition(this->x,this->y);
                this->speed_clock.restart();
            }
            if((this->x==this->x2 && this->y>=this->y2)){
                //line ends
                if(this->is_falling==true) this->is_falling=false;
                this->initialize();
                
            }else{
                WindowManager::getWindow().draw(this->bird_sprite);
            }
            
        }

        void draw( ){
            if(this->fall_start) {
                //change line of bird path to vertically down
                this->x1=this->x;
                this->y1=this->y;
                this->x2=this->x1;
                this->y2=WindowManager::getWindow().getSize().y;
                this->speed=1000;
                this->clock.restart();
                this->speed_clock.restart();
                this->fall_start=false;
            }
            this->update_along_line();
            this->play_sound();
        }

        void fall(){
            this->is_falling=true;
            this->fall_start=true;
        }

        void setPosition(float x, float y){
            this->bird_sprite.setPosition(x,y);
        }

        sf::FloatRect getGlobalBounds(){
            return this->bird_sprite.getGlobalBounds();
        }

        void play_sound(){
            int t=getRandomNumber(2000,3000);
            if( this->sound_clock.getElapsedTime().asMilliseconds()>=(t) ){
                if(!this->is_falling) this->sound.play();
                this->sound_clock.restart();
            }
        }
        

};

class Background{
    private:
        sf::Texture background_texture;
        sf::Sprite background_sprite;
    public:
        Background(){
            this->background_texture.loadFromFile("./assets/background.jpg");
            this->background_texture.setSmooth(true);
            this->background_sprite.setTexture(background_texture);
            auto textureSize=background_texture.getSize();
            this->background_sprite.setScale((float)WindowManager::getWindow().getSize().x/textureSize.x,(float)WindowManager::getWindow().getSize().y/textureSize.y); 
        }

        void draw(){
            WindowManager::getWindow().draw(this->background_sprite);
        }

};

class display_text{
    private:
        sf::Font font;
        sf::Text score_text , bird_dead_text , time_remaining_text , game_over_text;
    public:
        display_text(){

            this->font.loadFromFile("./font/arial.ttf");
            // Set the font
            this->score_text.setFont(font);
            this->bird_dead_text.setFont(font);
            this->time_remaining_text.setFont(font);
            this->game_over_text.setFont(font);
            // Set the character size
            this->score_text.setCharacterSize(24);
            this->time_remaining_text.setCharacterSize(24);
            this->bird_dead_text.setCharacterSize(24); 
            this->game_over_text.setCharacterSize(50);
             // Set the fill color
            this->score_text.setFillColor(sf::Color::Black);
            this->time_remaining_text.setFillColor(sf::Color::Black);
            this->bird_dead_text.setFillColor(sf::Color::Black);
            this->game_over_text.setFillColor(sf::Color::Red);
            // Set the text style
            this->score_text.setStyle(sf::Text::Bold );
            this->time_remaining_text.setStyle(sf::Text::Bold );
            this->bird_dead_text.setStyle(sf::Text::Bold );
            this->game_over_text.setStyle(sf::Text::Bold );
            //set position
            this->score_text.setPosition(0.f, 0.f);
            this->time_remaining_text.setPosition(WindowManager::getWindow().getSize().x/2.45, 0.f);
            this->bird_dead_text.setPosition(WindowManager::getWindow().getSize().x/1.3,0.f); 
            this->game_over_text.setPosition(WindowManager::getWindow().getSize().x/2.5,WindowManager::getWindow().getSize().y/2.5);
        }

        void update_score_text(int score){
            this->score_text.setString("SCORE : "+to_string(score));
        }

        void update_bird_dead_text(int bird_dead){
            this->bird_dead_text.setString("Birds shot : "+to_string(bird_dead));
        }

        void update_time_remaining_text(int time_duration){
            this->time_remaining_text.setString("TIME LEFT : "+to_string(time_duration)+"s");
        }

        void game_over_text_setString(string s){
            this->game_over_text.setString(s);
        }
        
        void draw_score_text(){
            WindowManager::getWindow().draw(this->score_text);
        }
        void draw_bird_dead_text(){
            WindowManager::getWindow().draw(this->bird_dead_text);
        }
        void draw_time_remaining_text(){
            WindowManager::getWindow().draw(this->time_remaining_text);
        }
        void draw_game_over_text(){
            WindowManager::getWindow().draw(this->game_over_text);
        }


};

int main() {
    
    cout << "Bird Hunt\nSFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;

    //create scope object , bird , background , display_text objects
    int n=5;
    Bird *b[n]; Scope s; Background bg; display_text dt;
    for(int i=0;i<n;i++) {
        b[i]=new Bird();       
    }

    //create clock
    sf::Clock clock;
    
    sf::SoundBuffer sound_buffer1,sound_buffer2;
    sf::Sound tick_tock_sound,cricket_chirping_sound;
    sound_buffer1.loadFromFile("./audio/clock_tick_tock.wav");
    sound_buffer1.loadFromFile("./audio/cricket_chirping.wav");
    tick_tock_sound.setBuffer(sound_buffer1);
    cricket_chirping_sound.setBuffer(sound_buffer2);
    
    //text number content
    int score=0,bird_dead=0, time_duration=10;

    // Main loop
    while (WindowManager::getWindow().isOpen()) {

        // Process events
        sf::Event event;
        while (WindowManager::getWindow().pollEvent(event)) {
            // Close the window if the close button is clicked
            if (event.type == sf::Event::Closed) {
                WindowManager::getWindow().close();
            }else if (WindowManager::getWindow().pollEvent(event) && event.type == sf::Event::MouseMoved) {
                //set scope obj pos to where  mouse is
                s.setPosition(event.mouseMove.x,event.mouseMove.y);
            }else if (event.type == sf::Event::MouseButtonReleased) {
                //left mouse mouse button clicked then 
                // Get mouse coordinates
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                s.play_sound(); //play gunshot sound
                for(int i=0;i<n;i++){
                    sf::FloatRect bounds=b[i]->getGlobalBounds();
                    if (bounds.contains(mouseX, mouseY)) {
                        b[i]->fall();
                        bird_dead++;
                        score+=10;
                        time_duration+=5;
                        break;
                    }
                }
            }
        }

        if(time_duration>0 && clock.getElapsedTime().asSeconds()>=1){
                time_duration--;
                tick_tock_sound.play();
                cricket_chirping_sound.play();
                clock.restart();
        }
        if(time_duration==0){
        
            sf::SoundBuffer sound_buffer;
            sf::Sound gameover_sound;
            sound_buffer.loadFromFile("./audio/game_over.wav");
            gameover_sound.setBuffer(sound_buffer);
            while(1){
                //display game over
                if (event.type == sf::Event::Closed) {
                    WindowManager::getWindow().close();
                }
                //update score and dead count 
                dt.update_score_text(score);
                dt.update_bird_dead_text(bird_dead);
                dt.update_time_remaining_text(0);
                dt.draw_bird_dead_text();
                dt.draw_score_text();
                dt.draw_time_remaining_text();

                dt.game_over_text_setString("Game Over!");
                dt.draw_game_over_text();
                
                if(time_duration==0){
                    //play game over only once so 
                    time_duration--;
                    gameover_sound.play();
                }
	        
                // Display the window content
        	WindowManager::getWindow().display();
        	
        	
            }
        }
         // Clear the window
        WindowManager::getWindow().clear();
        
       
        bg.draw();
        for(int i=0;i<n;i++) {b[i]->draw();}
        s.draw();

        //update display texts and draw em
        dt.update_score_text(score);
        dt.update_bird_dead_text(bird_dead);
        dt.update_time_remaining_text(time_duration);
        dt.draw_score_text();
        dt.draw_bird_dead_text();
        dt.draw_time_remaining_text();
        
        
        // Display the window content
        WindowManager::getWindow().display();
    }

    return 0;
}

