#include <iostream>
#include <string>
#include <fstream>
#include <vector>


#define DEBUG false

using namespace std;

//return amount of frames tht model have
int get_frames_amount(string file)
{
    std::ifstream source;
    source.open(file, ios::binary);
    int s = -1;
    if (source.is_open() )
    {
        char first_bytes[] = "    ";
        source.seekg(0, ios::beg);
        source.read(first_bytes, 4);
        if (strcmp(first_bytes, "IDP3") == 0)//md3 definition
        {
            if(DEBUG)
                cout << "correct file type" << endl;
            //md3 frames offset 4C(hex)/76(dec) bytes from the begining of the file
            source.seekg(76);
            s = source.get();
            source.close();
        }
    }
    else std::cout << "unable to open file " << file << endl;
    if (source.is_open())
        source.close();
    
    return s;
}

int main(int argc, const char** argv)
{
    //29 + terminating character for frame definition
    char alphabet[30] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\";

    if (argc > 2)
    {
        std::vector<std::string> args(argv, argv + argc);
        int frames_amount = -1;
        frames_amount = get_frames_amount(args[1]);
        if(DEBUG)
            cout << "model have " << frames_amount << " frames" << endl;

        string file = args[1];
        string sprite_name = args[2];

        while (sprite_name.length() > 4)
            sprite_name.pop_back();

        //chekc is it a valid chars
       
        //format sprite name here to use BIG letters
        for (int i = 0; i < 4; i++)
        {
            if (sprite_name[i])
            {
                if ((int(sprite_name[i]) >= 48 && int(sprite_name[i]) <= 57) || ((int(sprite_name[i]) >= 65 && int(sprite_name[i]) <= 90)))
                {
                    if (DEBUG)
                        cout << "leggit symbol, do nothing" << endl;
                }

                else if (int(sprite_name[i]) >= 97 && int(sprite_name[i]) <= 122)
                {
                    if (DEBUG)
                        cout << "replacing small letter with BIG" << endl;
                    char(sprite_name[i] -= 74);
                }

                else//outside of acceptable gzdoom sprite name symbols
                {
                    cout << "invalid symbol in sprite definition, changing it to 0" << endl;
                    char(sprite_name[i] += (-int(sprite_name[i]) + 48));//set it to 0/zero
                }
            }
        }

        //zero and lower mean something goes wrong
        if (frames_amount > 0)
        {
            //reduce sprite name size to 4 character
            if (sprite_name.size() > 4)
            {
                if(DEBUG)
                    cout << "sprite name too long, cutting some symbols" << endl;
                while (sprite_name.size() > 4)
                {
                    sprite_name.pop_back();
                }
            }

            //remove file extension with dot
            for (int i = 0; i < 4; i++)
                file.pop_back();

            std::ofstream model_definition;
            model_definition.open("modeldef." + file + ".txt", std::ios::out | std::ios::app);
            model_definition << "\n";
            model_definition << "model " + file + "\n" + "{\n";
            model_definition << "\tPath \"path to model\" \n";
            model_definition << "\tModel 0 \"model name\" \n";
            model_definition << "\tSkin 0  \"texture name\" \n";
            model_definition << "\tScale three numbers here \n";
            model_definition << "\n";

            model_definition << "\tUSEACTORROLL \\\\model would follow actors roll\n";
            model_definition << "\tUSEACTORPITCH \\\\model would follow actors pitch\n";
            model_definition << "\n";

            for (int i = 0; i < frames_amount; i++)
            {
                char frame = alphabet[(i % 29)];

                string index_str = to_string(i);

                string final_string = "\tFrameIndex " + sprite_name + " " + frame + " 0 " + index_str + "\n";
                model_definition << final_string;


                if (i > 0 && (i % 29) == 0)
                {
                    //0 = 48... 9 = 57
                    //A = 65...Z = 90
                    //[ = 91 \ = 92 ] = 93
                    char(sprite_name[3] ++);
                    if (int(sprite_name[3]) > 57 && int(sprite_name[3]) < 65)
                        char(sprite_name[3] += 7);
                    if (int(sprite_name[3]) > 90)
                    {
                        char(sprite_name[3] -= 26);
                        char(sprite_name[2] ++);

                        if (int(sprite_name[2]) > 57 && int(sprite_name[2]) < 65)
                            char(sprite_name[2] += 7);

                        if (int(sprite_name[2]) > 90)
                        {
                            char(sprite_name[2] -= 26);
                            char(sprite_name[1] ++);

                            if (int(sprite_name[1]) > 57 && int(sprite_name[1]) < 65)
                                char(sprite_name[1] += 7);

                            if (int(sprite_name[1]) > 90)
                            {
                                char(sprite_name[1] -= 26);
                                char(sprite_name[0] ++);

                                if (int(sprite_name[0]) > 57 && int(sprite_name[0]) < 65)
                                    char(sprite_name[0] += 7);

                                if (int(sprite_name[0]) > 90)
                                {
                                    cout << "out of empty sprite names, ZZZZ sprite name is 'max' sprite name in Gzdoom" << endl;
                                    cout << "export only " << i + 1 << " frames" << endl;
                                    goto exit_loop;
                                }
                            }
                        }
                    }
                }
            }
            cout << "all model frames successfully exported to modeldef" << endl;
        exit_loop:
            model_definition << "\n}";
        }

        else
            cout << "unsupported file format/model dont have animation(???????)" << endl;
    }
    else
        cout << "did not receive starting sprite name and/or model file" << endl;
    return 0;
}

