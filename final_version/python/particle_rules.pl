:- use_module(library(lists)).
:- set_prolog_flag(float_format, '%.3f').
:- dynamic particle/3.


% Поиск частиц по типу
find_particles_by_type(Type, Positions) :-
    findall((X, Y), particle(Type, X, Y), Positions).


% Предикат для фильтрации частиц
select_particles_of_type([], _, []).
select_particles_of_type([particle(Type, X, Y)|T], Type, [particle(Type, X, Y)|Filtered]) :-
    select_particles_of_type(T, Type, Filtered).
select_particles_of_type([particle(OtherType, _, _)|T], Type, Filtered) :-
    OtherType \= Type,
    select_particles_of_type(T, Type, Filtered).