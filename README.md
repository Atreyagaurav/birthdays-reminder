# Birthdays Reminder

## About
This program simply scans a file with birthdays informations and then displays the list with number before the next birthday.

compile with gcc, then use -h option for help.

## With Conky
Since I made it for conky, I'll share the conky setup too. Important thing is, keep the path of birthdays.tt file absolute for it to work with conky.
This is my conky setup for displaying the birthdays for upto 1 month in advance (color coded).
```
conky.text = [[
${color e43526}BIRTHDAYS:$color
${color 00ff00}${execi 60 ~/kool/Programming/C/birthdays/birthdays -t}$color
${color ffff00}${execi 60 ~/kool/Programming/C/birthdays/birthdays -l 1 -u 7}$color
${color ffffff}${execi 60 ~/kool/Programming/C/birthdays/birthdays -l 8 -u 30}$color
]]
```
## Aknowledgement
The BS and AD conversion codes are copied and slightly modified from the [Bipul Panthi's Repository](https://github.com/bpanthi977/calendar). Without it, I'd not have been able to support the BS date format. 

As for the idea, I nearly forgot the birthday of my friend, so some credit goes to him. 
