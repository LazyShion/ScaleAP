## Scalable Affinity Propagation for Massive Datasets
The implementation of ScaleAP proposed in AAAI'21.
If you use our software, **please cite the following paper**.

```
Hiroaki Shiokawa,  
"Scalable Affinity Propagation for Massive Datasets,"
Proceedings of the AAAI Conference on Artificial Intelligence, 35(11), 9639-9646, May, 2021.
```
``` 
@article{Shiokawa2021,
  title   = {{Scalable Affinity Propagation for Massive Datasets}},
  author  = {Hiroaki Shiokawa},
  journal = {Proceedings of the AAAI Conference on Artificial Intelligence},
  volume  = {35},
  number  = {11},
  pages={9639-9646},
  month   = {5},	
  year    = {2021},
  url={https://ojs.aaai.org/index.php/AAAI/article/view/17160},
}
```

### LICENSE
This software is released under the MIT license. Please read `LICENSE` for details.


## How to Use
### Requirements
ScaleAP requires the following softwares.
* gcc Version 9.2.0 (or later)

We have confirmed that our software works on the following environments.
* CentOS 8
* MacOS 11.4

### Build
1. Install *g++* on your environment.
2. Build the softwares.
```
$ make clean; make
```
3. If you can find `ScaleAP` and `convert`, the build has been successfully completed.


### Usage
#### Input file
The input file must include the number of objects, which is followd by a list of similarities among the objects.
By letting `sim(i,j)` be a similarity between objects i and j, the file should be formatted as follows:

``` sample_sim.txt
4	// # of objects
-0.11	// sim(1,1)
-0.12	// sim(1,2)
-0.13	// sim(1,3)
-0.14	// sim(1,4)
-0.21	// sim(2,1)
-0.22	// sim(2,2)
-0.23	// sim(2,3)
-0.24	// sim(2,4)
-0.31	// sim(3,1)
-0.32	// sim(3,2)
-0.33	// sim(3,3)
-0.34	// sim(3,4)
-0.41	// sim(4,1)
-0.42	// sim(4,2)
-0.43	// sim(4,3)
-0.44	// sim(4,4)
```

#### File conversion
`ScaleAP` reads the given similarities by using the CRS format, and this requires a file conversion process. 
To covert the input file (`sample_sim.txt`) into the CRS format (`sample_sim.bin`), we need to run `convert` like as follows:
``` convert
$ ./convert -i sample_graph.txt -o sample_graph.bin
```
`convert` requires two options, `-i` and `-o`, that specify names of the input file and the CRS formatted file, respectively.


#### Run ScaleAP
Finally, we can compute Affinity Propagation for `sample_sim.bin` by using `ScaleAP` like as follows:
```
$ ./ScaleAP -i sample_sim.bin 
```
`ScaleAP` has the following options.
|Option|Description|
|------|:----------|
|`-l <lambda>`|Set a dumping factor. `<lambda>` must be a float value between 0 and 1. (Default: `<lambda>`=0.5)|
|`-t <max_itr>`|Set a maximum number of iterations. `<max_itr>` should be a positive integer. (Default: `<max_itr>`=1000)|
|`-p <preference>`|Set a preference value. `<preference>` should be a float value. (Default: `<preference>`=0.5)|
|`-R`  |Display a clustering result of ScaleAP.|
|`-D`  |Display statistics (i.e., # of objects, and a running time.)|
|`-H`  |Display a help menu.|


##### Example
* Display the clustering result when we set `<preference>`=-0.22; each line indicates a pair of an object ID and a corresponding cluster ID.
In the following example, objects `0` and `1` are included in a cluster ID `1`.
```
$ ./ScaleAP -i sample_sim.bin -R -p -0.22
Object	Cluster
0	1
1	1
2	2
3	3
```

## Reference
* Hiroaki Shiokawa, "Scalable Affinity Propagation for Massive Datasets," Proceedings of the AAAI Conference on Artificial Intelligence, 35(11), 9639-9646, May, 2021.
* http://www.kde.cs.tsukuba.ac.jp/~shion/index.html.en
