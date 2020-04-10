## StatCov
Get and print GEL echange rate and live statistics about COVID-19 cases in Georgia.
Application runs only in terminal interface.

## Dependency
`openssl-devel` must be installed on the system to compile the project. Required for HTTPS requests.

Red-Hat family: `sudo dnf install openssl-devel`
Debian family: `sudo apt-get install libssl-dev`

## Installation
You can compile and install this project like any other unix dev package.
```
git clone https://github.com/kala13x/statcov.git
cd statcov
make
sudo make install
```

## Usage
After installation, just type 'statcov' command in the terminal, statistics will be printed below.

![alt tag](https://github.com/kala13x/statcov/blob/master/img/statcov.png)

### Feel free to fork
You can fork, modify and change the code unther the The MIT license. The project contains LICENSE file to see full license description.