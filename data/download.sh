# filenames
RAW_NYC_NEIGHS_FILENAME=d085e2f8d0b54d4590b1e7d1f35594c1pediacitiesnycneighborhoods.geojson
NYC_NEIGHS_FILENAME=nyc_neighs.geojson
TAXI_DATA_FILENAME=green_tripdata_2015-01.csv
TMP_FILENAME=temp.csv
POINTS_FILENAME=points.txt
POLYGON_LINES_FILENAME=polygon_lines.txt

# download
cat raw_data_urls.txt | xargs -n 1 -P 6 wget -c
mv $RAW_NYC_NEIGHS_FILENAME $NYC_NEIGHS_FILENAME  

# clean points
cp $TAXI_DATA_FILENAME $POINTS_FILENAME
cp $POINTS_FILENAME $TMP_FILENAME
cut -d, -f1-5,10- --complement $TMP_FILENAME > $POINTS_FILENAME
rm $TMP_FILENAME
sed -i '1d' $POINTS_FILENAME
sed -i 's/,/ /g' $POINTS_FILENAME
sed -i '/0 /d' $POINTS_FILENAME

# clean polygon lines
python clean_neighs.py > $POLYGON_LINES_FILENAME

# cat
cat points.txt polygon_lines.txt > data.txt
