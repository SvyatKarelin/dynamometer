const char MAIN_page[] = R"=====(
<head>
    <meta http-equiv="refresh" content="2; URL=/">
</head>

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
    <div style="text-align: center;"><button formaction="/REC" formmethod="get" name="session" type="submit" value="1">Srart</button></div>
    <div style="text-align: center;"><button formaction="/REC" formmethod="get" name="session" type="submit" value="0">Stop</button></div>
    <div style="text-align: center;">&nbsp;</div>
    <div style="text-align: center;"><label for="Axis-X">Select axis X</label><select name="Axis-X">
    <option <!--XDT--> value="DT">Time</option>
    <option <!--XRPS--> value="RPS">RadPS</option>
    <option <!--XRPM--> value="RPM">RotPM</option>
    <option <!--XANGACC--> value="ANGACC">AngAcc</option>
    <option <!--XMOMENT--> value="MOMENT">Moment</option>
    <option <!--XPOWER--> value="POWER">Power</option>
    </select></div>
    <div style="text-align: center;"><label for="Axis-Y">Select axis Y</label><select name="Axis-Y">
    <option <!--YDT--> value="DT">Time</option>
    <option <!--YRPS--> value="RPS">RadPS</option>
    <option <!--YRPM--> value="RPM">RotPM</option>
    <option <!--YANGACC--> value="ANGACC">AngAcc</option>
    <option <!--YMOMENT--> value="MOMENT">Moment</option>
    <option <!--YPOWER--> value="POWER">Power</option>
    </select></div>
    <div style="text-align: center;"><input formaction="/SVGSETTINGS" formmethod="get" type="submit" value="Apply" /></div>
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
    <div style="text-align: center;">[DT]</div>
    </td>
    </tr>
    </tbody>
    </table>
    </form>
</body>

</html>
)=====";