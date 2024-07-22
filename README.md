# ASCII-ART

![alt text](https://github.com/alexmateides/ascii-art/blob/main/examples/pic2.png) </br>

## Table of contents
<details>
  <summary></summary>
  <ol>
    <li>
      <a href="#about">About</a>
    </li>
    <li>
      <a href="#implemented-features">Implemented features</a>
      <ul>
        <li><a href="#loading">Loading</a></li>
        <li><a href="#filters">Filters</a></li>
        <li><a href="#image-output">Image output</a></li>
        <li><a href="#gui-controls">GUI controls</a></li>
      </ul>
    </li>
    <li>
      <a href="#deployment">Deployment</a>
      <ul>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#compile-and-launch">Compile and launch</a></li>
      </ul>
    </li>
    <li><a href="#controls">Controls</a></li>
    <li><a href="#config">Config</a></li>
    <li><a href="#classes-and-oop">Classes and OOP</a></li>
    <li><a href="#files">Files</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>


## About
I developed this application as a semestral project for my Programming and Algorithmics 2 (PA2) course at FIT CTU.

The objective was to develop an application that converts images into ASCII-art form. The application also had to
support some basic functions like switching images, presentation mode and most importantly image filters.

The app was developed with the help of SDL2 for graphics and Nlohmann's JSON library for config parsing.


## Implemented features

### Loading
- [X] **Images**
- [X] **RGB → grayscale ration**
- [X] **ASCII character set**
- [X] **Image file path**
- [X] **Output file**
- [X] **Preprocessing filters**
  - [X] **Individual images**
  - [X] **All images**

### Filters

- [X] **Brightness change**
- [X] **Vertical/Horizontal reflection**
- [X] **Negative**
- [X] **Rotation**
- [X] **Resize**
- [X] **Blur**
- [X] **Convolutions (matrices must be hardcoded)**

### Image output

- [X] **GUI**
- [X] **Console**
- [X] **File**

### GUI Controls

- [X] **Image cycling**
- [X] **Presenation mode**
- [X] **Filter application**

## Deployment

### Installation

**0. Install SDL2**

```sh
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-dev
```

**1. Clone the repo**

```sh
git clone https://github.com/alexmateides/ascii-art
```

### Compile and launch

**0. Change WD**

```sh
cd ascii-art
```

**1. Compile the project**

```sh
make compile
```

**2. Launch the app**

```sh
make run
```

or

```sh
./ascii-art (path to config)
```

**3. Clean the directory**

```sh
make clean
```

## Controls

### Image control

<kbd>←</kbd>/<kbd>→</kbd> &nbsp;&nbsp;**Cycle through images**</br>
<kbd>space</kbd> &nbsp;&nbsp;**Presentation mode**</br>
<kbd>K</kbd>&emsp;&emsp;&ensp;&nbsp;**Output current image into console**</br>
<kbd>L</kbd>&emsp;&emsp;&ensp;&nbsp;**Output all images into console**</br>
<kbd>O</kbd>&emsp;&emsp;&ensp;&nbsp;**Output current image into file**</br>
<kbd>P</kbd>&emsp;&emsp;&ensp;&nbsp;**Output all images into file**</br>

### Filters

<kbd>↑</kbd>/<kbd>↓</kbd> &nbsp;&nbsp;**Change brightness**</br>
<kbd>D</kbd>/<kbd>F</kbd> &nbsp;&nbsp;**Change size**</br>
<kbd>G</kbd>/<kbd>H</kbd> &nbsp;&nbsp;**Rotation**</br>
<kbd>N</kbd>&emsp;&emsp;&ensp;&nbsp;**Negative**</br>
<kbd>R</kbd>&emsp;&emsp;&ensp;&nbsp;**Reflection around Y**</br>
<kbd>T</kbd>&emsp;&emsp;&ensp;&nbsp;**Reflection around X**</br>
<kbd>C</kbd>&emsp;&emsp;&ensp;&nbsp;**Convolutional blur**</br>

## Config

The application is configured using a json file that is provided as a parameter during launch.</br></br>
**Example:**

```sh
./ascii-art "./assets/config/config1.json"
```

This file needs to have a specific format that can be seen in `ascii-art/assets/config` <br>

### Config description

```
{
  "alphabet":"@#%*+=-:. ",             # Characters that will be used for rendering (darkest on left)
  "img_folder":"./assets/pictures/"    # Path to image directory
  "img_paths": [                       # Paths to individual images inside image directory
    "img1.png",
    "img2.jpg"
  ],

  "equation": [                        # Ratio for RGB -> grayscale conversion
    0.299,
    0.587,
    0.114
  ],

  "output_path":"./out.txt",           # Path and name of output text file

  "defaultFilters":{                   # Default filter config (will be bound on keys)
    "brightness": ["A",10],          
    "rotation": 90,
    "resize": 1.2,
    "blur": 4
  },

  "filters": {                         # Preprocessing filters (will be applied at start)
    "image1.jpg":[
      {
        "filterName": "resize",
        "factor": 1.5
      },
      {
        "filterName":"negative"
      },
      {
        "filterName":"blur",
        "strength": 5
      }
    ],
    "image2.png": [
      {
      "filterName": "resize",
      "factor": 0.8
      }
    ]
  }
}
```

### Filter syntax
```
"imgName":
[         
    {
      "filterName":(name),
      (argName):(argValue)
    },
    {
      "filterName":(name2),
      (argName2):(argValue2)   
    }
]
```
- If you use **"all"** as imgName, the filters will be applied to all images </br>
- Filters can have 0-2 arguments

### Filter argument table
| filter                | filterName           | argName       | argValue         |
|-----------------------|----------------------|---------------|------------------|
| resize                | resize               | factor        | float (0+)       |
| blur                  | blur                 | strength      | int   (1+)       |
| rotation              | rotation             | angle         | int              |
| brightness            | brightness           | type<br>value | A/R<br>int/float |
| vertical reflection   | reflectionVertical   | NONE          | NONE             |
| horizontal reflection | reflectionHorizontal | NONE          | NONE             |
| negative              | negative             | NONE          | NONE             |


## Classes and OOP

### CLoader
Loads and processes data from configuration file, especially syntactic check of configuration file data

### CData
Stores data, mainly images and filter configuration data and handles preprocessing before app start (grayscale conversion and filter application)

### CAtlas
Glyph atlas (pre-rendered ASCII sprites) that allows significantly faster rendering in GUI which allows the application to run smoothly, improving user experience

### CFilter
Abstract class that handles image filtering.
- **filter()** Each child class implements its own filter. This allows for good modularity and adding more filters in the future. The class design also allows "chaining" of filters → _nextFilter is applied automatically after
- **copy()** Returns deep copy as a shared_ptr. Is used for filter chaining
- **copyInverse()** Returns shared_ptr to a CFilter instance with inverse characteristics
- **addChain()** Adds new filter to the end of the chain


### CApp
Is responsible for GUI (SDL2 based). Allows the user to apply additional filters and view their effects. It also allows the user to send the output in ASCII text form to either console or file. There is also a presentation mod that automatically cycles through the images.

## Files

```md
ascii-art
│
├── assets                  # Configuration and input files
│   ├── config              # Configuration files
│   ├── fonts               # Different fonts for GUI rendering
│   └── pictures            # Pictures directory
│
├── doc                     # Doxygen documentation
│
├── examples                # Sample files and ASCII results
│   └── configTest          # Unit tests for syntax checking
│
├── libs                    # Nlohmann's JSON library
│
├── src                     # Source code with header and implementation files
│
├── LICENSE                 # Open source license
│
├── Makefile                # Instructions for program compilation and execution
│
└── README.md               # This file
```

## License
Distributed under the MIT License. See LICENSE for more information.

## Contact
**Alexander Mateides** - alex.mateides@gmail.com - [LinkedIn](https://www.linkedin.com/in/alexander-mateides-138136285/)
