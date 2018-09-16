---
title: "Elevator - Going Up!"
date: 2018-08-09T00:13:02+10:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

Sometimes

you

get

bored

and

need

to

have

some

relaxation

so

you

listen

to

elevator

music

while

scrolling

your

facebook

newsfeed

BUT

what

if

you

want

to

look

at

the

firt

post

on

the

page!

You'd

have

to

scroll

up

and

up

and

up

and

up

n

up

and

up

and

upppppppppppp

...

...

You

could

press

the

home

key

but

what

about

pressing

this

conveniently

looking

elevator

icon

---------------->


<script src="/js/elevator.min.js"></script>

<script>
window.onload = function() {
  var elevator = new Elevator({
    element: document.querySelector('.elevator'),
    mainAudio: '../../js/elevator.js/elevator.mp3',
    endAudio: '../../js/elevator.js/ding.mp3',
    duration: 10000 // milliseconds
  });
}
</script>

<style>
.elevator {
    text-align: center;
    cursor: pointer;
    width: 140px;
    margin: auto;
}

.elevator:hover {
    opacity: 0.7;
}

.elevator svg {
    width: 40px;
    height: 40px;
    display: block;
    margin: auto;
    margin-bottom: 5px;
}
</style>
{{% center %}}
<div class="elevator">
<svg class="sweet-svg" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" x="0px" y="0px" viewBox="0 0 100 100" enable-background="new 0 0 100 100" xml:space="preserve" height="100px" width="100px">
<path d="M70,47.5H30c-1.4,0-2.5,1.1-2.5,2.5v40c0,1.4,1.1,2.5,2.5,2.5h40c1.4,0,2.5-1.1,2.5-2.5V50C72.5,48.6,71.4,47.5,70,47.5z   M47.5,87.5h-5v-25h5V87.5z M57.5,87.5h-5v-25h5V87.5z M67.5,87.5h-5V60c0-1.4-1.1-2.5-2.5-2.5H40c-1.4,0-2.5,1.1-2.5,2.5v27.5h-5  v-35h35V87.5z"/>
<path d="M50,42.5c1.4,0,2.5-1.1,2.5-2.5V16l5.7,5.7c0.5,0.5,1.1,0.7,1.8,0.7s1.3-0.2,1.8-0.7c1-1,1-2.6,0-3.5l-10-10  c-1-1-2.6-1-3.5,0l-10,10c-1,1-1,2.6,0,3.5c1,1,2.6,1,3.5,0l5.7-5.7v24C47.5,41.4,48.6,42.5,50,42.5z"/>
</svg>
</div>
{{% /center %}}

_/* Found this script a few years back, showed it to my friend today! */_