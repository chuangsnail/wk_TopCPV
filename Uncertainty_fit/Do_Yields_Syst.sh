#! bin/bash
#before sh the file we need to '$cmsenv'
cd /wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit && eval `scramv1 runtime -sh`

./Draw_Yields.py -s btag -v up -c mu -o O3
./Draw_Yields.py -s btag -v up -c mu -o O6
./Draw_Yields.py -s btag -v up -c mu -o O12
./Draw_Yields.py -s btag -v up -c mu -o O14

./Draw_Yields.py -s btag -v down -c mu -o O3
./Draw_Yields.py -s btag -v down -c mu -o O6
./Draw_Yields.py -s btag -v down -c mu -o O12
./Draw_Yields.py -s btag -v down -c mu -o O14

./Draw_Yields.py -s btag -v up -c el -o O3
./Draw_Yields.py -s btag -v up -c el -o O6
./Draw_Yields.py -s btag -v up -c el -o O12
./Draw_Yields.py -s btag -v up -c el -o O14

./Draw_Yields.py -s btag -v down -c el -o O3
./Draw_Yields.py -s btag -v down -c el -o O6
./Draw_Yields.py -s btag -v down -c el -o O12
./Draw_Yields.py -s btag -v down -c el -o O14


./Draw_Yields.py -s pu -v up -c mu -o O3
./Draw_Yields.py -s pu -v up -c mu -o O6
./Draw_Yields.py -s pu -v up -c mu -o O12
./Draw_Yields.py -s pu -v up -c mu -o O14

./Draw_Yields.py -s pu -v down -c mu -o O3
./Draw_Yields.py -s pu -v down -c mu -o O6
./Draw_Yields.py -s pu -v down -c mu -o O12
./Draw_Yields.py -s pu -v down -c mu -o O14

./Draw_Yields.py -s pu -v up -c el -o O3
./Draw_Yields.py -s pu -v up -c el -o O6
./Draw_Yields.py -s pu -v up -c el -o O12
./Draw_Yields.py -s pu -v up -c el -o O14

./Draw_Yields.py -s pu -v down -c el -o O3
./Draw_Yields.py -s pu -v down -c el -o O6
./Draw_Yields.py -s pu -v down -c el -o O12
./Draw_Yields.py -s pu -v down -c el -o O14


./Draw_Yields.py -s lepsf -v up -c mu -o O3
./Draw_Yields.py -s lepsf -v up -c mu -o O6
./Draw_Yields.py -s lepsf -v up -c mu -o O12
./Draw_Yields.py -s lepsf -v up -c mu -o O14

./Draw_Yields.py -s lepsf -v down -c mu -o O3
./Draw_Yields.py -s lepsf -v down -c mu -o O6
./Draw_Yields.py -s lepsf -v down -c mu -o O12
./Draw_Yields.py -s lepsf -v down -c mu -o O14

./Draw_Yields.py -s lepsf -v up -c el -o O3
./Draw_Yields.py -s lepsf -v up -c el -o O6
./Draw_Yields.py -s lepsf -v up -c el -o O12
./Draw_Yields.py -s lepsf -v up -c el -o O14

./Draw_Yields.py -s lepsf -v down -c el -o O3
./Draw_Yields.py -s lepsf -v down -c el -o O6
./Draw_Yields.py -s lepsf -v down -c el -o O12
./Draw_Yields.py -s lepsf -v down -c el -o O14


./Draw_Yields.py -s JER -v up -c mu -o O3
./Draw_Yields.py -s JER -v up -c mu -o O6
./Draw_Yields.py -s JER -v up -c mu -o O12
./Draw_Yields.py -s JER -v up -c mu -o O14

./Draw_Yields.py -s JER -v down -c mu -o O3
./Draw_Yields.py -s JER -v down -c mu -o O6
./Draw_Yields.py -s JER -v down -c mu -o O12
./Draw_Yields.py -s JER -v down -c mu -o O14

./Draw_Yields.py -s JER -v up -c el -o O3
./Draw_Yields.py -s JER -v up -c el -o O6
./Draw_Yields.py -s JER -v up -c el -o O12
./Draw_Yields.py -s JER -v up -c el -o O14

./Draw_Yields.py -s JER -v down -c el -o O3
./Draw_Yields.py -s JER -v down -c el -o O6
./Draw_Yields.py -s JER -v down -c el -o O12
./Draw_Yields.py -s JER -v down -c el -o O14


./Draw_Yields.py -s JES -v up -c mu -o O3
./Draw_Yields.py -s JES -v up -c mu -o O6
./Draw_Yields.py -s JES -v up -c mu -o O12
./Draw_Yields.py -s JES -v up -c mu -o O14

./Draw_Yields.py -s JES -v down -c mu -o O3
./Draw_Yields.py -s JES -v down -c mu -o O6
./Draw_Yields.py -s JES -v down -c mu -o O12
./Draw_Yields.py -s JES -v down -c mu -o O14

./Draw_Yields.py -s JES -v up -c el -o O3
./Draw_Yields.py -s JES -v up -c el -o O6
./Draw_Yields.py -s JES -v up -c el -o O12
./Draw_Yields.py -s JES -v up -c el -o O14

./Draw_Yields.py -s JES -v down -c el -o O3
./Draw_Yields.py -s JES -v down -c el -o O6
./Draw_Yields.py -s JES -v down -c el -o O12
./Draw_Yields.py -s JES -v down -c el -o O14

