#include <iostream>
#include "bmplib.h"
#include <unistd.h>
using namespace std;
class photo_editor{
private:
    unsigned char imgGS[SIZE][SIZE];
    unsigned char imgGS2[SIZE][SIZE];
public:
    void load_image(){
        string image_name,path = "\\imgs\\";
        cout << "Please enter the name of the image to process (NOTE: Must be in imgs folder, without bmp):";
        cin >> image_name;
        path += image_name;
        path += ".bmp";
        char cwd[PATH_MAX];
        readGSBMP(strcat(getcwd(cwd,sizeof(cwd)),path.c_str()),imgGS);
    }
    void show_image(){
        showGSBMP(imgGS);
    }
    void save_image () {
        // the image will be saved at cmake files in tmp folder
        string imageFileName, path = "\\tmp\\";
        // Get gray scale result image target file name
        cout << "Enter the result image name(saved in tmp folder at cmake) : ";
        cin >> imageFileName;
        //adding the extension
        imageFileName += ".bmp";
        path += imageFileName;
        char cwd[PATH_MAX];
        writeGSBMP(strcat(getcwd(cwd,sizeof(cwd)),path.c_str()), imgGS);
    }
    bool is_digits(string& str)
    {
        for (char ch : str) {
            if (!(ch >= 48 && ch <= 57)) {
                return false;
            }
        }
        return true;
    }
    // filter 1 : black and white filter
    void Black_n_white_filter(){
        for(int i = 0 ; i < SIZE ; i++){
            for(int j = 0 ; j < SIZE ; j++){
                /*looping on each pixel and if the value is greater than 127
                 * we make that pixel white else we make it black */
                if(imgGS[i][j] > 127){
                    imgGS[i][j] = 255;
                }
                else{
                    imgGS[i][j] = 0;
                }
            }
        }
    }

    // filter 2 : invert filter
    void invert_filter()
    {
        for (int i = 0; i < SIZE; i++)
        {
            // Reflects pixels ( new pixel = 255 - old pixel)
            for (int j = 0; j < SIZE; j++)
                imgGS[i][j] = 255 - imgGS[i][j];
        }
    }

    // filter 3 : Merge Filter
    void Merge_Filter()
    {
        // uploading the second image
        string image_name,path = "\\imgs\\";
        cout << "Please enter the name of the second image to process (NOTE: Must be in imgs folder, without .bmp):";
        cin >> image_name;
        path += image_name;
        path += ".bmp";
        char cwd[PATH_MAX];
        readGSBMP(strcat(getcwd(cwd,sizeof(cwd)),path.c_str()),imgGS2);

        // Merging two photos requires
        // every pixel to be the average of
        // the 2 pixels from each photo

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j< SIZE; j++) {
                imgGS[i][j] = (imgGS[i][j] + imgGS2[i][j]) /2;
            }
        }
    }

    // filter 4 : flip image filter
    void flip(){
        // choosing the direction
        cout << "Enter 1 for vertical flip or 2 for horizontal flip:";
        int direction;cin>>direction;
        if(direction == 1){
            /* we iterate over the columns and rows of the photo,
            * focusing on the upper half of the image.
            * during this process, it swaps each row with its corresponding
            * row on the other side, creating a vertical mirror effect.
            */
            for(int i = 0 ; i < SIZE/2 ; i++){
                for (int j = 0; j <SIZE ; ++j) {
                    swap(imgGS[i][j] , imgGS[SIZE-i][j]);
                }
            }
        }

        else if(direction == 2){
            /* we iterate over the columns and rows of the photo,
            * focusing on the left half of the image.
            * during this process, it swaps each column with its corresponding
            * column on the other side, creating a horizontal mirror effect.
            */
            for(int i = 0 ; i < SIZE ; i++){
                for (int j = 0; j <SIZE/2 ; ++j) {
                    swap(imgGS[i][j] , imgGS[i][SIZE-j]);
                }
            }
        }
        else
        {
            cout << "wrong direction, please try again\n";
            flip();
        }
    }


    // filter 5 : Darken and Lighten Image
    void Darken_and_Lighten_Image ()
    {
        // choose the filter wanted
        cout <<"select a number :\n(1) lighten\n(2) darken\n";
        int x;
        cin >> x;

        // this is the lightening filter
        if (x == 1)
        {
            // 1. subtract full light value (255) - the original light value
            // 2. add the result to the original light value
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    imgGS[i][j] = imgGS[i][j] + (255 - imgGS[i][j])/2;
                }

            }
        }
        else
        {
            // divide the original by two
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    imgGS[i][j] = imgGS[i][j]/2;
                }

            }
        }
    }

    // filter 6
    void Rotate_Image()
    {
        unsigned char imgGS1[SIZE][SIZE];
        cout << "choose angle :\n(.90\t.180\t.270)\n";
        int x;
        cin >> x;
        if (x == 90)
        {
            for (int i  = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                    imgGS1[j][255 - i] = imgGS[i][j];
            }
        }
        else if (x == 180)
        {
            for (int i  = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                    imgGS1[255 - i][255 - j] = imgGS[i][j];
            }
        }
        else
        {
            for (int i  = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                    imgGS1[255 - j][i] = imgGS[i][j];
            }
        }

        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                imgGS[i][j] = imgGS1[i][j];
    }

    //filter 8 : enlarge image
    void enlarge_image()

    {

        // first of all choose the quarter you want to enlarge
        cout << "Choose the number of the required quarter :\n1. quarter one\n2. quarter two\n3. quarter three\n4. quarter four\n";
        int quarter;
        cin >> quarter;
        char mod_imgGS[2 * SIZE][2 * SIZE];

        //enlarging image dimensions to double
        for (int i = 0; i < 2 * SIZE; i++)
        {
            for (int j = 0; j < 2 * SIZE; j++)
            {
                mod_imgGS[i][j] = imgGS[i / 2][j / 2];
            }
        }

        // detecting the quarter based on the input
        if (quarter == 1)
        {
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    imgGS[i][j] = mod_imgGS[i][j];
                }
            }
        }
        else if (quarter == 2)
        {
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    imgGS[i][j] = mod_imgGS[i][255 + j];
                }
            }
        }
        else if (quarter == 3)
        {
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    imgGS[i][j] = mod_imgGS[255 + i][j];
                }
            }
        }
        else
        {
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    imgGS[i][j] = mod_imgGS[255 + i][255 + j];
                }
            }
        }
    }

    void menu2() {
        cout << "Make your choice :\n"
                "0-  Exit\n"
                "1-  show image\n"
                "2-  save image\n"
                "3-  return to filter menu\n";
        string choice_s;
        cin >> choice_s;
        int choice = 0;
        if (is_digits(choice_s)) {
            choice = stoi(choice_s);
        } else {
            cout << "Invalid choice, please try again\n";
            menu2();
        }
        if(choice == 1) {
            show_image();
        }
        else if(choice == 2) {
            save_image();
        }
        else if(choice == 3) {
            menu();
        }
        else if (choice == 0) {
            return;
        }

        else {
            cout << "Invalid choice, please try again\n";

        }
        menu2();
    }

    void menu() {
        cout <<
             "Make your choice :\n"
             "0-  Exit\n"
             "1-  Black & White Filter\n"
             "2-  Invert Filter\n"
             "3-  Merge Filter \n"
             "4-  Flip Image\n"
             "5-  Darken and Lighten Image \n"
             "6-  Rotate Image\n"
             "7-  Detect Image Edges \n"
             "8-  Enlarge Image\n"
             "9-  Shrink Image\n"
             "10- Mirror 1/2 Image\n"
             "11- Shuffle Image\n"
             "12- Blur Image\n"
             "13- Crop Image\n"
             "14- Skew Image Right  \n"
             "15- Skew Image Up  \n"

            "note : only the first six filters work\n";
        string choice_s;
        cin >> choice_s;
        int choice = 0;
        if (is_digits(choice_s)) {
            choice = stoi(choice_s);
        } else {
            cout << "Invalid filter, please try again\n";
            menu();
        }
        if (choice == 1) {
            Black_n_white_filter();
        } else if (choice == 2) {
            invert_filter();

        } else if (choice == 3) {
            Merge_Filter();
        } else if (choice == 4) {
            flip();
        } else if (choice == 5) {
            Darken_and_Lighten_Image();
        } else if (choice == 6) {
            Rotate_Image();
        } else if (choice == 7) {

        } else if (choice == 8) {
            enlarge_image();
        } else if (choice == 9) {

        } else if (choice == 10) {

        } else if (choice == 11) {

        } else if (choice == 12) {

        } else if (choice == 13) {

        } else if (choice == 14) {

        } else if (choice == 15) {

        } else if (choice == 0)
        {
            return;
        }
        else {
            cout << "Invalid filter, please try again\n";
            menu();
        }
        menu2();
    }

};
int main() {
    photo_editor photo;
    photo.load_image();
    photo.menu();
}
