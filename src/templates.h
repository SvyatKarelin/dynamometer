#pragma once
const char SETTINGS_page[] = R"=====(
<body>
  <form>
  <table style="height: 14px; width: 50%; border-collapse: collapse; border-style: outset;" border="1">
  <tbody>
  <tr style="height: 260px;">
  <td style="width: 50%; height: 14px;" rowspan="2">
    <div style="text-align: center;">
		[SVGREP]
    </div>
    </td>
    <td style="width: 50%; height: 14px;" rowspan="2">
    <div style="text-align: center;"><button formaction="/rec" formmethod="get" name="session" type="submit" value="1">Srart</button></div>
    <div style="text-align: center;">&nbsp;</div>
    <div style="text-align: center;"><label for="Axis-X">Select axis X</label><select name="Axis-X">
    <option <!--XTIME--> value="TIME">Time</option>
    <option <!--XRPS--> value="RPS">RadPS</option>
    <option <!--XRPM--> value="RPM">RotPM</option>
    <option <!--XANGACC--> value="ANGACC">AngAcc</option>
    <option <!--XMOMENT--> value="MOMENT">Moment</option>
    <option <!--XPOWER--> value="POWER">Power</option>
    </select></div>
    <div style="text-align: center;"><label for="Axis-Y">Select axis Y</label><select name="Axis-Y">
    <option <!--YTIME--> value="TIME">Time</option>
    <option <!--YRPS--> value="RPS">RadPS</option>
    <option <!--YRPM--> value="RPM">RotPM</option>
    <option <!--YANGACC--> value="ANGACC">AngAcc</option>
    <option <!--YMOMENT--> value="MOMENT">Moment</option>
    <option <!--YPOWER--> value="POWER">Power</option>
    </select></div>
    <div style="text-align: center;">&nbsp;</div>
    <div style="text-align: center;"><input formaction="/apply_settings" formmethod="get" type="submit" value="Apply" /></div>
  </td>
  </tr>
  </tbody>
  </table>
  </form>
</body>

</html>
)=====";

const char MAIN_page[] = R"=====(
<head>
    <meta http-equiv="refresh" content="2; URL=/session">
</head>

<body>
  <form>
  <table style="height: 14px; width: 50%; border-collapse: collapse; border-style: outset;" border="1">
  <tbody>
  <tr style="height: 260px;">
  <td style="width: 50%; height: 14px;" rowspan="2">
  <div style="text-align: center;">[SVGREP]</div>
  </td>
  <td style="width: 50%; height: 14px;" rowspan="2">
  <div style="text-align: center;"><button formaction="/rec" formmethod="get" name="session" type="submit" value="0">Stop</button></div>
  <div style="text-align: center;">&nbsp;</div>
  <div style="text-align: center;"><button formaction="/settings" formmethod="get" type="submit">Change settings</button></div>
  <div style="text-align: center;">&nbsp;</div>
  <div style="text-align: center;">RPM:</div>
  <div style="text-align: center;">[RPM]</div>
  <div style="text-align: center;">RadPS:</div>
  <div style="text-align: center;">[RPS]</div>
  <div style="text-align: center;">Angle Acc:</div>
  <div style="text-align: center;">[ANGACC]</div>
  <div style="text-align: center;">Moment:</div>
  <div style="text-align: center;">[MOMENT]</div>
  <div style="text-align: center;">Power:</div>
  <div style="text-align: center;">[POWER]</div>
  <div style="text-align: center;">Delta time:</div>
  <div style="text-align: center;">[DELTATIME]</div>
  </td>
  </tr>
  </tbody>
  </table>
  </form>
</body>

</html>
)=====";

const char SvgTemplate[] = R"=====(
<svg xmlns="http://www.w3.org/2000/svg" width="415" height="215" viewBox="0 0 415 215">
    <text x="0" y="8" class="small" transform="rotate(270)" fill = "black" text-anchor="end" font-size = "10">[AXY]</text>
    <text x="415" y="213" class="small" fill = "black" text-anchor="end" font-size = "10">[AXX]</text>
    <svg x="15" y="0" width="400" height="200" viewBox="0 0 400 200" fill="none">
        <!--L-->
        <path stroke="blue" stroke-linecap = "round" d="M 0 200"/><!--P-->
        <!--C-->
    </svg>
</svg>
)=====";

const char HLine[] = R"=====(
<path d="M0 [Y] H 400" stroke = "grey"/>
<text x="0" y="[Y]" class="small" fill = "black" font-size = "10">[VAL]</text>
)=====";

const char VLine[] = R"=====(
<path d="M[X] 0 V 400" stroke = "grey"/>
<text x="[X]" y="200" class="small" fill = "black" font-size = "10">[VAL]</text>
)=====";