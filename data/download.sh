RAW_NYC_NEIGHS_FILENAME=d085e2f8d0b54d4590b1e7d1f35594c1pediacitiesnycneighborhoods.geojson
NYC_NEIGHS_FILENAME=nyc_neighs.geojson
TAXI_DATA_FILENAME=green_tripdata_2015-01.csv
TMP_FILENAME=temp.csv

cat raw_data_urls.txt | xargs -n 1 -P 6 wget -c
mv $RAW_NYC_NEIGHS_FILENAME $NYC_NEIGHS_FILENAME  
cp $TAXI_DATA_FILENAME $TMP_FILENAME
cut -d, -f1-5,10- --complement $TMP_FILENAME > $TAXI_DATA_FILENAME
rm $TMP_FILENAME
sed -i '1d' $TAXI_DATA_FILENAME
sed -i 's/,/ /g' $TAXI_DATA_FILENAME
sed -i '/0 /d' $TAXI_DATA_FILENAME
python clean_neighs.py > clean_neighs.txt
