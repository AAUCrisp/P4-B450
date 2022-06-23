 |-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
  |-|-|-|-|-|-  Main program for Control Unit  -|-|-|-|-|-|
  |-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
  |-|-|                                               |-|-|
  |-|-|  For 4th semester project of ComTek Group 450 |-|-|
  |-|-|             Spring Semester 2022              |-|-|
  |-|-|                                               |-|-|
  |-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|




   Arguments Available for this program is:
  __________________________________________________________

  ----------------------------------------------------------
  ----  Disable Start of the Signal Monitoring Process  ----
  ----                                                  ----
   Call Arguments:
      -m
      -monitor
      -monitoring


  ----------------------------------------------------------
  ----       Use RAM Grid for Command Conversion        ----
  ----                                                  ----
   Call Argument:
      -g
      -grid


  ----------------------------------------------------------
  ----          Force Communication Technology          ----
  ----                                                  ----
   Arguments market with * can be written sequencially
   without needing to call -v / -verbose again

   Call Argument:
      -t <extra_1>
      -tech <extra_1>
      -technology <extra_1>

   Accepted Extra Arguments:
      no extra              - Force use of both (incl. GSV)
      b / both            * - Force use of both (incl. GSV)
      w / wifi            * - Force use of WiFi (incl. GSV)
      l / lte             * - Force use of LTE (incl. GSV)


  ----------------------------------------------------------
  ----                Change Axis Sizes                 ----
  ----                                                  ----
   Call Argument:
      -x <int>              - For the X-axis
      -y <int>              - For the Y-axis

   Accepted Arguments:
      Any int               - Int of the size you want


  ----------------------------------------------------------
  ----            Signal Monitor Sleep Time             ----
  ----                                                  ----
   Call Argument:
      -s <int>
      -sleep <int>

   Accepted Arguments:
      Any int               - Seconds of delay you want


  ----------------------------------------------------------
  ----    Setting for Disabling Hex-Char Coordinates    ----
  ----                                                  ----
   Call Argument:
      -h
      -hex


  ----------------------------------------------------------
  ---- Verbose, to write out everthing that is going on ----
  ----                                                  ----
   Arguments market with * can be written sequencially
   without needing to call -v / -verbose again

   Call Argument:
      -v <extra_1> <extra_2> <extra_3> <extra_4>
      -verbose <extra_1> <extra_2> <extra_3> <extra_4>

   Accepted Extra Arguments:
      no extra              - Prints everything
      m / msg / message   * - Prints "important" messages
      i / in              * - Prints from incoming traffic
      o / out             * - Prints from outgoing traffic
      g / gsv             * - Prints from the GSV system


