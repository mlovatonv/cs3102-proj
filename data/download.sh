cat raw_data_urls.txt | xargs -n 1 -P 6 wget -c
mv d085e2f8d0b54d4590b1e7d1f35594c1pediacitiesnycneighborhoods.geojson nyc_neighs.geojson
