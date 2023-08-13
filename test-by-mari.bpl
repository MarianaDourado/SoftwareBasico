function f1
def
enddef
return ci0
end

function f2 pi1
def
var vi1
vet va2 size ci30
var vi3
enddef
return ci-1
end

function f3 pa1 pi2
def
vet va1 size ci10
vet va2 size ci20
var vi3
reg vr4
var vi5
reg vr6
enddef
return ci5
end

function f4 pi1 pi2
def
enddef
return pi1
end

function f5 pi1 pi2 pi3
def
var vi1
reg vr2
var vi3
enddef
vi1 = ci1 
vr2 = vi1 
vi1 = pi1 + vr2 
vr2 = vi1 * ci-5 
vi3 = call f4 vr1 pa1
return vi3
end

function f6 pi1
def
var vi1
vet va2 size ci10
var vi3
enddef
vi1 = ci3
vi3 = ci4
if vi1 ne vi3
vi1 = ci0
endif
if vi1 lt vi3
get va2 index ci8 to vi1
endif
if vi3 le
return vi1
endif
if pi1 le ci10
return ci0
endif
if ci-10 gt ci10
get va2 index ci2 to vi3
endif
return ci-1
end

function f7 pi1 pa2
def
var vi1
vet va2 size ci10
reg vr3
enddef
vi1 = pi1 + ci1
set va2 index ci5 with ci2 # va2[5] = 2
set pa2 index ci0 with vi1 # pa2[0] = vi1
get va2 index ci8 to vi1 # vi1 = va2[8]
get pa2 index ci4 to vr3 # vr3 = pa2[4]
return pi1
end
