# local_hub
ການສ້າງ private cloud ສຳລັບການເກັບຂໍ້ມູນ sensors ໃນ local ເພື່ອເປັນແຜນໃນການພັດທະນາລະບົບ IoT ໃຫ້ສົມບູນຂື້ນ, ນອກຈາກນັ້ນຍັງສາມາດນຳຂໍ້ມູນໄປພັດທະນາເປັນ Machine Learning ແລະ AI ໃນອະນາຄົດໄດ້

## Install Local hub requirement
**_All the requirements software are available in the following step_**
#### Install mosquitto (MQTT Broker)

`sudo apt install mosquitto mosquitto-clients`

This command will install both mosquitto and mosquitto-clients

**enable mosquitto run when system is booted**

`sudo systemctl enable mosquitto`

The broker should now be running in the backend

**mosquitto service status**

`sudo systemctl status mosquitto`

This should be showing the following output:

```pi@melon:~ $ sudo systemctl status mosquitto
sudo: unable to resolve host melon: Name or service not known
● mosquitto.service - Mosquitto MQTT v3.1/v3.1.1 Broker
   Loaded: loaded (/lib/systemd/system/mosquitto.service; enabled; vendor preset: enabled)
   Active: active (running) since Tue 2020-07-28 11:14:08 +07; 4min 16s ago
     Docs: man:mosquitto.conf(5)
           man:mosquitto(8)
 Main PID: 2395 (mosquitto)
    Tasks: 1 (limit: 4915)
   CGroup: /system.slice/mosquitto.service
           └─2395 /usr/sbin/mosquitto -c /etc/mosquitto/mosquitto.conf

Jul 28 11:14:08 melon systemd[1]: Starting Mosquitto MQTT v3.1/v3.1.1 Broker...
Jul 28 11:14:08 melon systemd[1]: Started Mosquitto MQTT v3.1/v3.1.1 Broker.
```

_you can better understand [mosquitto](https://mosquitto.org/)_

## Install Influxdb (MQTT time-series database)

Adding the key will allow the package manager on Raspbian to search the repository and verify the packages its installing.

`wget -qO- https://repos.influxdata.com/influxdb.key | sudo apt-key add -`

Now enter the following command to add the InfluxDB repository to the sources list.

`echo "deb https://repos.influxdata.com/debian buster stable" | sudo tee /etc/apt/sources.list.d/influxdb.list`

Run the following command on your Raspberry Pi to update the package list.

`sudo apt update`

To install InfluxDB to our Raspberry Pi, all we need to do is run the command below.

`sudo apt install influxdb`

Run the following two commands to enable InfluxDB to start at boot on your Raspberry Pi.

```sudo systemctl unmask influxdb
sudo systemctl enable influxdb
```

To start up the InfluxDB server, we will need to run the following command. The service manager will then start up the service and begin monitoring it.

`sudo systemctl start influxdb`

_you can better understand [influxdb](https://docs.influxdata.com/platform/introduction)_
