//cl /EHsc /std:c++17
// NEEDS C++ STD 17!
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

std::string encode(const std::string& data) {
    std::string buffer;
    buffer.reserve(data.size());
    for(size_t pos = 0; pos != data.size(); ++pos) {
        switch(data[pos]) {
            case '&':  buffer.append("&amp;");       break;
            case '\"': buffer.append("&quot;");      break;
            case '\'': buffer.append("&apos;");      break;
            case '<':  buffer.append("&lt;");        break;
            case '>':  buffer.append("&gt;");        break;
            default:   buffer.append(&data[pos], 1); break;
        }
    }
    return buffer;
}

int main(int argc, char* argv[]){
    std::cout << "Creating Lists" << std::endl;
    for(int x = 1;x<argc;x++){
        if(!std::filesystem::exists(argv[x])) continue;
        std::filesystem::path outfile{argv[x]};
        outfile.replace_extension(".xspf");
        std::cout << "Working on \"" << argv[x] << "\"\n\tCreate \"" << outfile << "\"\n";
        if(std::filesystem::exists(outfile)){
            std::cerr << "\tOutfile already exists!\n";
            continue;
        }
        std::ifstream txtfile(argv[x]); //open a file to perform read operation using file object
        if (txtfile.is_open()){   //checking whether the file is open
           std::ofstream newfile(outfile);
           if(!newfile.is_open()){
            std::cerr << "\tCan't Create File\n";
            continue;
           }

           newfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<playlist xmlns=\"http://xspf.org/ns/0/\" xmlns:vlc=\"http://www.videolan.org/vlc/playlist/ns/0/\" version=\"1\">\n\t<title>Playlist</title>\n\t<trackList>";

           std::string tp;
           size_t trackcount = 0;
           while(getline(txtfile, tp)){ //read data from file object and put it into string.
                if(tp.size() < 6) continue;
                newfile << "<track><location>" << encode(tp) << "</location><extension application=\"http://www.videolan.org/vlc/playlist/0\"><vlc:id>" << trackcount++ << "</vlc:id><vlc:option>network-caching=1000</vlc:option></extension></track>";
           }
           txtfile.close(); //close the file object.
           newfile << "</trackList>\n\t<extension application=\"http://www.videolan.org/vlc/playlist/0\">";
           for(size_t t = 0;t<trackcount;t++){
            newfile << "<vlc:item tid=\"" << t << "\"/>";
           }
           newfile << "</extension>\n</playlist>";
           newfile.close();
        }else
            std::cerr << "\tCan't Open File\n";
    }
    return 0;
}
