{-# LANGUAGE FlexibleInstances #-}

module Main where

import Data.List (sortOn)
import Data.Maybe (catMaybes)
import Control.Monad.Writer

data Axis = X | Y deriving (Eq, Show)
data Point = Point Int Int deriving (Eq, Ord)

instance Show Point where
  show (Point x y) = "(" ++ show x ++ ", " ++ show y ++ ")"

data KdTree =
    Empty
  | Node Axis Point KdTree KdTree
  deriving (Eq, Show)

-- Получение координаты по оси
getCoord :: Axis -> Point -> Int
getCoord X (Point x _) = x
getCoord Y (Point _ y) = y

-- Смена оси
nextAxis :: Axis -> Axis
nextAxis X = Y
nextAxis Y = X

-- Построение сбалансированного дерева из списка точек
buildKdTree :: Axis -> [Point] -> KdTree
buildKdTree _ [] = Empty
buildKdTree axis pts =
  let sorted = sortOn (getCoord axis) pts
      midIdx = length sorted `div` 2
      median = sorted !! midIdx
      left = take midIdx sorted
      right = drop (midIdx + 1) sorted
  in Node axis median (buildKdTree (nextAxis axis) left) (buildKdTree (nextAxis axis) right)

-- Проверка попадания точки в прямоугольник
inRange :: Point -> Point -> Point -> Bool
inRange (Point lx ly) (Point rx ry) (Point x y) =
  x >= lx && x <= rx && y >= ly && y <= ry

-- Поиск в диапазоне с логированием только точек, попавших в диапазон
searchRange :: Point -> Point -> KdTree -> Writer [String] [Point]
searchRange _ _ Empty = return []
searchRange lower upper (Node axis p left right) = do
  let coord = getCoord axis p
      (minB, maxB) = (getCoord axis lower, getCoord axis upper)
      here = if inRange lower upper p
             then do
               tell ["Found point " ++ show p ++ " in range"]
               return [p]
             else return []
  goLeft <- if minB <= coord
            then searchRange lower upper left
            else return []
  goRight <- if maxB >= coord
             then searchRange lower upper right
             else return []
  return (here ++ goLeft ++ goRight)

-- Основная программа с логированием
main :: IO ()
main = do
  let testPoints = [
        Point 2 3, Point 4 1, Point 5 5, Point 3 3, 
        Point 1 2, Point 6 4, Point 0 0, Point 2 5,
        Point 4 4, Point 3 1
        ]
      tree = buildKdTree X testPoints
  
  putStrLn "=== KD-дерево построено ==="
  print tree

  putStrLn "\n=== Тест 1: Поиск в диапазоне (0,0)-(6,6) ==="
  let (result1, log1) = runWriter $ searchRange (Point 0 0) (Point 6 6) tree
  putStrLn $ "Найдено: " ++ show (length result1) ++ " точек: " ++ show result1
  putStrLn "Лог поиска:"
  mapM_ putStrLn log1

  putStrLn "\n=== Тест 2: Поиск в вертикальной полосе (X:2-4) ==="
  let (result2, log2) = runWriter $ searchRange (Point 2 (-100)) (Point 4 100) tree
  putStrLn $ "Найдено: " ++ show (sortOn (\(Point x y) -> (x, y)) result2)
  putStrLn "Лог поиска:"
  mapM_ putStrLn log2

  putStrLn "\n=== Тест 3: Поиск в горизонтальной полосе (Y:3-5) ==="
  let (result3, log3) = runWriter $ searchRange (Point (-100) 3) (Point 100 5) tree
  putStrLn $ "Найдено: " ++ show (sortOn (\(Point x y) -> (x, y)) result3)
  putStrLn "Лог поиска:"
  mapM_ putStrLn log3

  putStrLn "\n=== Тест 4: Пустой диапазон ==="
  let (result4, log4) = runWriter $ searchRange (Point 10 10) (Point 20 20) tree
  putStrLn $ "Найдено: " ++ show result4
  putStrLn "Лог поиска:"
  mapM_ putStrLn log4