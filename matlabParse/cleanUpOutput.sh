cd /home/radion/Yandex.Disk/Ilmenau/Thesis/Coding/matlabParse/

sed -i 's/strong//g'  $1
sed -i 's/[>,<,/]//g'  $1
sed -i 's/Data =//g'  $1
sed -i 's/Scenario =//g'  $1
sed -i '/^$/d'  $1
