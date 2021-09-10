<?xml version="1.0" encoding="UTF-8"?>
<simconf>
  <project EXPORT="discard">[APPS_DIR]/mrm</project>
  <project EXPORT="discard">[APPS_DIR]/mspsim</project>
  <project EXPORT="discard">[APPS_DIR]/avrora</project>
  <project EXPORT="discard">[APPS_DIR]/serial_socket</project>
  <project EXPORT="discard">[APPS_DIR]/collect-view</project>
  <project EXPORT="discard">[APPS_DIR]/powertracker</project>
  <simulation>
    <title>My simulation</title>
    <speedlimit>1.0</speedlimit>
    <randomseed>123456</randomseed>
    <motedelay_us>1000000</motedelay_us>
    <radiomedium>
      org.contikios.cooja.radiomediums.DirectedGraphMedium
      <edge>
        <source>1</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>2</radio>
          <ratio>0.97</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>1</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>3</radio>
          <ratio>0.83</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>2</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>1</radio>
          <ratio>0.96</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>2</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>4</radio>
          <ratio>0.86</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>2</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>5</radio>
          <ratio>0.78</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>3</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>1</radio>
          <ratio>0.77</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>3</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>4</radio>
          <ratio>0.71</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>3</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>5</radio>
          <ratio>0.88</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>4</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>2</radio>
          <ratio>0.75</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>4</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>3</radio>
          <ratio>0.98</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>4</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>6</radio>
          <ratio>0.85</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>4</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>7</radio>
          <ratio>0.82</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>5</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>2</radio>
          <ratio>0.73</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>5</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>3</radio>
          <ratio>0.72</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>5</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>6</radio>
          <ratio>0.76</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>5</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>7</radio>
          <ratio>0.83</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>6</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>4</radio>
          <ratio>0.98</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>6</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>5</radio>
          <ratio>0.73</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>6</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>8</radio>
          <ratio>0.85</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>7</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>4</radio>
          <ratio>0.87</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>7</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>5</radio>
          <ratio>0.9</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>7</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>8</radio>
          <ratio>0.72</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>8</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>6</radio>
          <ratio>0.79</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
      <edge>
        <source>8</source>
        <dest>
          org.contikios.cooja.radiomediums.DGRMDestinationRadio
          <radio>7</radio>
          <ratio>0.9</ratio>
          <signal>-10.0</signal>
          <lqi>105</lqi>
          <delay>0</delay>
          <channel>-1</channel>
        </dest>
      </edge>
    </radiomedium>
    <events>
      <logoutput>40000</logoutput>
    </events>
    <motetype>
      org.contikios.cooja.contikimote.ContikiMoteType
      <identifier>mtype273</identifier>
      <description>mote</description>
      <source>[CONTIKI_DIR]/examples/ipv6/disjoint/node.c</source>
      <commands>make -j clean TARGET=cooja MAKE_WITH_DEMO=1 MAKE_WITH_STATIC_SCHEDULER=1 MAKE_WITH_PACKET_REPLICATION_AND_ELIMINATION=1 MAKE_WITH_SEQUENCE_NUMBER=1 MAKE_WITH_ALTERNATIVE_PARENT_SELECTION=1
make -j node.cooja TARGET=cooja MAKE_WITH_DEMO=1  MAKE_WITH_STATIC_SCHEDULER=1 MAKE_WITH_PACKET_REPLICATION_AND_ELIMINATION=1 MAKE_WITH_SEQUENCE_NUMBER=1 MAKE_WITH_ALTERNATIVE_PARENT_SELECTION=1</commands>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Battery</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiVib</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRS232</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiBeeper</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.RimeAddress</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiIPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiRadio</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiButton</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiPIR</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiClock</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiLED</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiCFS</moteinterface>
      <moteinterface>org.contikios.cooja.contikimote.interfaces.ContikiEEPROM</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <symbols>false</symbols>
    </motetype>
    <mote>
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>44.94503584702244</x>
        <y>23.75479823665682</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiMoteID
        <id>1</id>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiRadio
        <bitrate>250.0</bitrate>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiEEPROM
        <eeprom>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</eeprom>
      </interface_config>
      <motetype_identifier>mtype273</motetype_identifier>
    </mote>
    <mote>
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>22.926613557189256</x>
        <y>5.783205928525549</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiMoteID
        <id>2</id>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiRadio
        <bitrate>250.0</bitrate>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiEEPROM
        <eeprom>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</eeprom>
      </interface_config>
      <motetype_identifier>mtype273</motetype_identifier>
    </mote>
    <mote>
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>65.09405290117786</x>
        <y>3.377290428972529</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiMoteID
        <id>3</id>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiRadio
        <bitrate>250.0</bitrate>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiEEPROM
        <eeprom>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</eeprom>
      </interface_config>
      <motetype_identifier>mtype273</motetype_identifier>
    </mote>
    <mote>
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>24.328361324493674</x>
        <y>-25.37145342577743</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiMoteID
        <id>4</id>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiRadio
        <bitrate>250.0</bitrate>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiEEPROM
        <eeprom>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</eeprom>
      </interface_config>
      <motetype_identifier>mtype273</motetype_identifier>
    </mote>
    <mote>
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>64.3749022240531</x>
        <y>-25.23759928131257</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiMoteID
        <id>5</id>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiRadio
        <bitrate>250.0</bitrate>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiEEPROM
        <eeprom>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</eeprom>
      </interface_config>
      <motetype_identifier>mtype273</motetype_identifier>
    </mote>
    <mote>
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>24.947984358905767</x>
        <y>-55.13203561662659</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiMoteID
        <id>6</id>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiRadio
        <bitrate>250.0</bitrate>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiEEPROM
        <eeprom>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</eeprom>
      </interface_config>
      <motetype_identifier>mtype273</motetype_identifier>
    </mote>
    <mote>
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>66.14486103478531</x>
        <y>-55.69123546406539</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiMoteID
        <id>7</id>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiRadio
        <bitrate>250.0</bitrate>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiEEPROM
        <eeprom>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</eeprom>
      </interface_config>
      <motetype_identifier>mtype273</motetype_identifier>
    </mote>
    <mote>
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>46.54929423801621</x>
        <y>-75.79548644532962</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiMoteID
        <id>8</id>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiRadio
        <bitrate>250.0</bitrate>
      </interface_config>
      <interface_config>
        org.contikios.cooja.contikimote.interfaces.ContikiEEPROM
        <eeprom>AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</eeprom>
      </interface_config>
      <motetype_identifier>mtype273</motetype_identifier>
    </mote>
  </simulation>
  <plugin>
    org.contikios.cooja.plugins.SimControl
    <width>280</width>
    <z>0</z>
    <height>160</height>
    <location_x>409</location_x>
    <location_y>28</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.Visualizer
    <plugin_config>
      <moterelations>true</moterelations>
      <skin>org.contikios.cooja.plugins.skins.IDVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.GridVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.DGRMVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.TrafficVisualizerSkin</skin>
      <viewport>4.263801435752849 0.0 0.0 4.263801435752849 -12.118164165730668 372.974835154657</viewport>
    </plugin_config>
    <width>400</width>
    <z>3</z>
    <height>602</height>
    <location_x>5</location_x>
    <location_y>1</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.ScriptRunner
    <plugin_config>
      <script>/*
 * Example showing how to reference and interact with surrounding
 * COOJA plugins from a test script.
 * The code looks up three common plugins and, if found, performs some
 * simple plugin-specific task.
 */

/* Started plugins are available from the GUI object */

randomNumberSeed = 1; // same number as one used in contiki/cooja
totalSimulationDurationInMicroSec = 30*60*1000*1000; // 30*60 sec
radioRatioRandomSetterMinQualityInPercent = 70; // e.g. 70% -&gt; 70
radioRatioRandomSetterMaxQualityInPercent = 100; // e.g. 70% -&gt; 70
radioRatioRandomSetterStartTimeInMicroSec = 1*60*1000*1000; // 10minutes
radioRatioRandomSetterIntervalTimeInMicroSec = 3*60*1000*1000; // 3minutes

packet_offset=0;
next_powerTracker_time = 0;
first_time_pT = 0;
last_time_pT = 0; 
powerTrackerPlugin = sim.getCooja().getPlugin("PowerTracker");



function XorGen(seed) {
  var me = this;

  // Set up generator function.
  me.next = function() {
    var w = me.w,
        X = me.X, i = me.i, t, v;
    // Update Weyl generator.
    me.w = w = (w + 0x61c88647) | 0;
    // Update xor generator.
    v = X[(i + 34) &amp; 127];
    t = X[i = ((i + 1) &amp; 127)];
    v ^= v &lt;&lt; 13;
    t ^= t &lt;&lt; 17;
    v ^= v &gt;&gt;&gt; 15;
    t ^= t &gt;&gt;&gt; 12;
    // Update Xor generator array state.
    v = X[i] = v ^ t;
    me.i = i;
    // Result is the combination.
    return (v + (w ^ (w &gt;&gt;&gt; 16))) | 0;
  };

  function init(me, seed) {
    var t, v, i, j, w, X = [], limit = 128;
    if (seed === (seed | 0)) {
      // Numeric seeds initialize v, which is used to generates X.
      v = seed;
      seed = null;
    } else {
      // String seeds are mixed into v and X one character at a time.
      seed = seed + '\0';
      v = 0;
      limit = Math.max(limit, seed.length);
    }
    // Initialize circular array and weyl value.
    for (i = 0, j = -32; j &lt; limit; ++j) {
      // Put the unicode characters into the array, and shuffle them.
      if (seed) v ^= seed.charCodeAt((j + 32) % seed.length);
      // After 32 shuffles, take v as the starting w value.
      if (j === 0) w = v;
      v ^= v &lt;&lt; 10;
      v ^= v &gt;&gt;&gt; 15;
      v ^= v &lt;&lt; 4;
      v ^= v &gt;&gt;&gt; 13;
      if (j &gt;= 0) {
        w = (w + 0x61c88647) | 0;     // Weyl.
        t = (X[j &amp; 127] ^= (v + w));  // Combine xor and weyl to init
										// array.
        i = (0 == t) ? i + 1 : 0;     // Count zeroes.
      }
    }
    // We have detected all zeroes; make the key nonzero.
    if (i &gt;= 128) {
      X[(seed &amp;&amp; seed.length || 0) &amp; 127] = -1;
    }
    // Run the generator 512 times to further mix the state before using it.
    // Factoring this as a function slows the main generator, so it is just
    // unrolled here. The weyl generator is not advanced while warming up.
    i = 127;
    for (j = 4 * 128; j &gt; 0; --j) {
      v = X[(i + 34) &amp; 127];
      t = X[i = ((i + 1) &amp; 127)];
      v ^= v &lt;&lt; 13;
      t ^= t &lt;&lt; 17;
      v ^= v &gt;&gt;&gt; 15;
      t ^= t &gt;&gt;&gt; 12;
      X[i] = v ^ t;
    }
    // Storing state as object members is faster than using closure
	// variables.
    me.w = w;
    me.X = X;
    me.i = i;
  }

  init(me, seed);
}

function XorGenCopy(f, t) {
  t.i = f.i;
  t.w = f.w;
  t.X = f.X.slice();
  return t;
};

function xor4096(seed, opts) {
  if (seed == null) seed = +(new Date);
  var xg = new XorGen(seed),
      state = opts &amp;&amp; opts.state,
      prng = function() { return (xg.next() &gt;&gt;&gt; 0) / 0x100000000; };
  prng.double = function() {
    do {
      var top = xg.next() &gt;&gt;&gt; 11,
          bot = (xg.next() &gt;&gt;&gt; 0) / 0x100000000,
          result = (top + bot) / (1 &lt;&lt; 21);
    } while (result === 0);
    return result;
  };
  prng.int32 = xg.next;
  prng.quick = prng;
  if (state) {
    if (state.X) XorGenCopy(state, xg);
    prng.state = function() { return XorGenCopy(xg, {}); }
  }
  return prng;
}

nextRandom = xor4096(randomNumberSeed);

function getRandomIntInclusive(min, max) {
	min = Math.ceil(min);
	max = Math.floor(max);
	return Math.floor(nextRandom() * (max - min + 1)) + min; //The maximum is inclusive and the minimum is inclusive 
};

findEdge = function(fromMoteId, toMoteId){
	var radioMedium = sim.getRadioMedium();
	for (edgeIndex in radioMedium.getEdges()) {
		var edge = radioMedium.getEdges()[edgeIndex];
		var src = edge.source.getMote().getID();
		var dst = edge.superDest.radio.getMote().getID();
		if (src == fromMoteId &amp;&amp; dst == toMoteId) {
			//log.log("findEdge "+fromMoteId +"-&gt;"+ toMoteId+":"+edge+"\n");
			return edge;
		}
	};
	//log.log("findEdge "+fromMoteId +"-&gt;"+ toMoteId+":null\n");
	return null;
};

setEdge = function(fromMoteId, toMoteId, overwrite, ratio, signal, lqi){
	//log.log("setEdge "+fromMoteId +"-&gt;"+ toMoteId+", overwrite:"+overwrite+", ratio:"+ratio+", signal:"+signal+", lqi:"+lqi+"\n");
	var radioMedium = sim.getRadioMedium();
	var edge = findEdge(fromMoteId, toMoteId);
	var destinationRadio = null;
	if(edge != null){
		destinationRadio = edge.superDest;
	} else {
		if(overwrite)
			destinationRadio = new org.contikios.cooja.radiomediums.DGRMDestinationRadio(sim.getMoteWithID(toMoteId).getInterfaces().getRadio());
		else{
			return;
//			throw ("There is no existing edge between "+fromMoteId +"-&gt;"+ toMoteId+"\n");
		}
	}
	
	destinationRadio.ratio = ratio;
	destinationRadio.signal = signal;
	destinationRadio.lqi = lqi;
	
	if(edge == null &amp;&amp; overwrite){
		edge = new org.contikios.cooja.radiomediums.DirectedGraphMedium.Edge(
				sim.getMoteWithID(fromMoteId).getInterfaces().getRadio(), destinationRadio
				);
		radioMedium.addEdge(edge);
	}
	
	radioMedium.requestEdgeAnalysis(); //This just sets a flag as done by addEdge - No harm in doing it again
};

getAllMoteIds = function(){
	var motesArray = sim.getMotes();
	var motesIDs = [];
	for(motesArrayIndex in motesArray)
		motesIDs.push(motesArray[motesArrayIndex].getID());
	return motesIDs;
};

setAllMoteIdRadioLinks = function(moteId, overwrite, ratio, signal, lqi){
	var allMoteIds = getAllMoteIds();
	for(otherMoteIdIndex in allMoteIds) {
		var otherMoteId = allMoteIds[otherMoteIdIndex];
		if(otherMoteId != moteId){
			setEdge(moteId, otherMoteId, overwrite, ratio, signal, lqi);
			setEdge(otherMoteId, moteId, overwrite, ratio, signal, lqi);
		}
	};
};

radioRatioRandomSetterNextThresholdInMicroSec = radioRatioRandomSetterStartTimeInMicroSec;
radioRatioRandomSetter = function(){
	if(time &gt;= radioRatioRandomSetterNextThresholdInMicroSec){
		var allMoteIds = getAllMoteIds();
		for(moteIdIndex1 in allMoteIds)
			for(moteIdIndex2 in allMoteIds)
				if(moteIdIndex1 != moteIdIndex2){
					var randomRatio = getRandomIntInclusive(radioRatioRandomSetterMinQualityInPercent, radioRatioRandomSetterMaxQualityInPercent)*1.0/100.0;
					setEdge(allMoteIds[moteIdIndex1], allMoteIds[moteIdIndex2], false, randomRatio, -10.0, 105);
				}
		
	//	setAllMoteIdRadioLinks(1, false, radioRatioSetterRootQualityInRatio, -10.0, 105);
		
		radioRatioRandomSetterNextThresholdInMicroSec += radioRatioRandomSetterIntervalTimeInMicroSec;
	}
}

checkEndFunction = function() {
	if(time &gt;= totalSimulationDurationInMicroSec){
		log.log(totalSimulationDurationInMicroSec+"||Simulation|END\n");
		SCRIPT_TIMEOUT();
		return true;
	}else
		return false;
};

powerTracker = function(){

//	  log.log("RadioStatistics Start\n");
	  statsStr = powerTrackerPlugin.radioStatistics();
//		  log.log(statsStr+"\n");
	  statsLines = statsStr.trim().split( '\n');
	  lineFunc = function(line){
          if(line.contains("Contiki")){
              return time+"|"+line.replace(/Contiki_([0-9]+) (.*)/, "$1|PowerTracker: $2");
          } else{
              return time+"|0|"+line;
          }
		  
	  };
	  statsFomattedLines = statsLines.map(lineFunc);
	  
	  for(lineIndex=0;lineIndex &lt; statsFomattedLines.length;lineIndex++)
		  log.log(statsFomattedLines[lineIndex]+"\n");
};

print_log = function(){
   i = 0; 
   timeslots = "";
   if(msg.contains("rpl_set_alternative_parent.alternative_parent:")){
       log.log(time + "|" + id + "|" + msg + "\n");
   }
   if(msg.contains("Sending a multicast-DIO")){
       log.log(time + "|" + id + "|" + msg + "\n");
   }
   if(msg.contains("Sending unicast-DIO")){
       log.log(time + "|" + id + "|" + msg + "\n");
   }
   if(msg.contains("rpl_set_preferred_parent.preferred_parent:")){
       log.log(time + "|" + id + "|" + msg + "\n");
   }
   if(msg.contains("CA:")){
       log.log(time + "|" + id + "|" + msg + "\n");
   }
   if(msg.contains("tx") || msg.contains("rx")){
      for(i = 33; i &lt; 297 ; i++){
          timeslots = "link-0-297-"+i+"-";
		  if(msg.contains(timeslots)){
		      log.log(time + "|" + id + "|" + msg + "\n");
		  }
	  }
   }
};

var printStream=null;
try {
	printStream=new java.io.PrintStream(new java.io.FileOutputStream(new java.io.File("/home/tlagos/messages.txt")), true, "UTF-8");
} catch (e) {
	e.printStackTrace();
}

TIMEOUT(900000000000,printStream.close());
packet_list = []
packet_values = []
packet_tx_time = 0
while (true) {
    if(packet_tx_time == 1 &amp;&amp; msg.contains("tx")){
        if(msg.contains("link-0-33-9")){
            packet_values[0] = packet_number
            packet_values[1] = parseInt(time)
            if(packet_list.length &lt; 1){
                packet_list.push(packet_values)
                //log.log("data push " + packet_list[0][0].toString()+ "|" +  packet_list[0][1].toString() + "\n")
            }
            else{
                packet_lost = packet_list.shift()
                //log.log(packet_lost[0][0].toString() + "|-1 \n")
            } 
            packet_tx_time = 0
        }
        if(msg.contains("link-0-33-11")){
            packet_values[0] = packet_number
            packet_values[1] = parseInt(time)
            if(packet_list.length &lt; 1){
                packet_list.push(packet_values)
                //log.log("data push " + packet_list[0][0].toString()+ "|" +  packet_list[0][1].toString() + "\n")
            }
            else{
                packet_lost = packet_list.shift()
                //log.log(packet_lost[0][0].toString() + "|-1 \n")
            } 
            packet_tx_time = 0
        }
    }
    if(msg.contains("Hello")){
       if(msg.contains("Sending data")){
            packet_tx_time = 1
            //log.log(msg.substring(msg.indexOf("Hello") + 6, msg.length) + "\n")
            packet_number = parseInt(msg.substring(msg.indexOf("Hello") + 6, msg.length)) 
            //   if(packet_list.length &lt; 5){
            //       log.log("Adding " + packet_number.toString() + " to queue \n")
            //        packet_list.push(packet_number)
            //   }
            //   else{
            //       packet_lost = packet_list.shift()
            //       log.log(packet_lost.toString() + "|-1 \n")
            //   }    
       }
       else{
            if(msg.contains("DATA recv")){
                packet_recived = parseInt(msg.substring(msg.indexOf("Hello") + 6, msg.indexOf("' from")))
                //log.log("packet rcv " + packet_recived.toString() + "\n")
                for(index = 0; index &lt; packet_list.length; index++){
                    if(packet_list[index][0] == packet_recived){
                        
                        delay =  Math.round((parseInt(time) - packet_list[index][1])/1000)
                        printStream.println(packet_recived.toString() + "|" + delay.toString())
          
                        packet_list.splice(index,1)
                    }
                }
            }
       } 
       
       //log.log(time + "|" + id + "|" + msg + "\n");
       if(msg.contains("Sending data")){
           packet_offset += 1;
       }
    }
    //print_log()
	//if(!checkEndFunction()){
        //if(time &gt;= 90000000 &amp;&amp; first_time_pT == 0){
           // powerTracker();
        //    first_time_pT = 1; 
        //}
        //if(packet_offset == 1001 &amp;&amp; last_time_pT == 0 ){
        //    powerTracker();
        //    last_time_pT = 1; 
        //}
		radioRatioRandomSetter();
		YIELD();
	//}
}</script>
      <active>true</active>
    </plugin_config>
    <width>1010</width>
    <z>2</z>
    <height>470</height>
    <location_x>726</location_x>
    <location_y>52</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.TimeLine
    <plugin_config>
      <mote>0</mote>
      <mote>1</mote>
      <mote>2</mote>
      <mote>3</mote>
      <mote>4</mote>
      <mote>5</mote>
      <mote>6</mote>
      <mote>7</mote>
      <showRadioRXTX />
      <showRadioHW />
      <showLEDs />
      <zoomfactor>500.0</zoomfactor>
    </plugin_config>
    <width>1853</width>
    <z>1</z>
    <height>237</height>
    <location_x>4</location_x>
    <location_y>685</location_y>
  </plugin>
</simconf>

