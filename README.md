
# Visualization of the Isabel hurricane

## Clone 

```bash
git clone --recursive --depth=1 git@github.com:shetr/viz_isabel.git
```

## Download input data

Run Git Bash in the data folder. To download only one time point run:

```bash
./wget-isabeldata-selected.sh
```

To download all time points run:

```bash
./wget-isabeldata-all.sh
```

## Build

```bash
cmake -S . -B build/Deploy -DCMAKE_BUILD_TYPE=Release -DENABLE_DEPLOY=true
cmake --build build/Deploy --target viz -j 8
```

## Run

```bash
.\viz.exe
```

## Dataset attribution

The authors will like to thank Bill Kuo, Wei Wang, Cindy Bruyere, Tim Scheitlin, and Don Middleton of the U.S. National Center for Atmospheric Research (NCAR), and the U.S. National Science Foundation (NSF) for providing the Weather Research and Forecasting (WRF) Model simulation data of Hurricane Isabel.