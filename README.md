# Bird Hunt SFML

A simple C++ game developed using object-oriented programming principles and the SFML library. The game was created and tested in a VirtualBox Ubuntu 20.04 environment with SFML version 2.5.1.<br/>
demo-https://youtu.be/mvnkNuuuDnc<br/>
sfml tutorial link:https://www.sfml-dev.org/tutorials/2.5/  ,  https://www.sfml-dev.org/documentation/2.5.1/ <br />
## Instructions:

### Prerequisites:

- VirtualBox with Ubuntu 20.04

### Getting Started:

1. **Clone the Repository:**

    ```bash
    git clone https://github.com/avalon22022002/bird-hunt-sfml
    cd bird-hunt-sfml
    ```

2. **Install SFML Library:**

    ```bash
    sudo apt-get update
    sudo apt-get install libsfml-dev
    ```

3. **Verify SFML Version:**

    ```bash
    pkg-config --modversion sfml-all
    ```
    sfml version used while creating this project == 2.5.1 if u receive any errors ahead try installing this specific version..

4. **Compile and Run the Game:**

    ```bash
    g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    ./a.out
    ```

### Enjoy playing the Bird Hunt game!

Feel free to customize the content further to suit your preferences.
