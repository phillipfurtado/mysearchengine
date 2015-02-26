#Description

Vector Space Model based search engine with Image Search (SDLC descriptor).

#Build

This c++ project depends of libxml2 library. (http://xmlsoft.org/downloads.html)

The sdlc.py project depends of Pillow library. (http://pillow.readthedocs.org/)


A set of files are required in order to proper build and run this project, below a directory infrastruture model is presented.

    mysearchengine /
                  /data/
                  /colecaoDafitiPosthaus/
                                        /all image collection...
                  /queries/
                          /all queries images and text files...
                  /relevants/
                            /relevants xmls...
                  /textDescDafitiPosthaus.xml



> $g++ -std=c++0x src/SearchEngine.cpp src/Document.cpp src/Term.cpp src/QueryDescriptor.cpp src/Relevant.cpp src/Produto.cpp -I /usr/include/libxml2 -lxml2 -g -o mysearchengine

#Run

To evaluate all queries
> $./mysearchengine

To search for a word
> $./mysearchengine -q [word]
