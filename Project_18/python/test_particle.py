import sys
sys.path.append("C:/Users/User/Desktop/C++/Project_18/python")
import particle

class TestParticle(particle.Particle):
    def __init__(self, x, y, size):
        super().__init__(x, y, size)
        
    def move(self, orientation, speed):
        self.x += speed * orientation
        
    def clone(self):
        return TestParticle(self.x, self.y, self.size)

def run_test(test_name, test_func):
    """Вспомогательная функция для запуска тестов с выводом в терминал"""
    print(f"[Тест] {test_name}... ", end="", flush=True)
    try:
        test_func()
        print(" Успешно!")
    except AssertionError as e:
        print(" Ошибка!")
        print(f"   Подробности: {str(e)}")
        raise

def main():
    print("\n=== Начало тестирования ===")
    
    # Тест 1: Создание объекта
    def test_creation():
        p = TestParticle(2.5, 3.7, 1.0)
        assert p.x == 2.5, f"Ожидалось x=2.5, получено {p.x}"
        assert p.y == 3.7, f"Ожидалось y=3.7, получено {p.y}"
        assert p.size == 1.0, f"Ожидалось size=1.0, получено {p.size}"
    
    run_test("Создание объекта", test_creation)

    # Тест 2: Изменение свойств
    def test_properties():
        p = TestParticle(2.5, 3.7, 1.0)
        p.x = 5.0
        p.size = 2.5
        assert p.x == 5.0, f"Ожидалось x=5.0, получено {p.x}"
        assert p.size == 2.5, f"Ожидалось size=2.5, получено {p.size}"
    
    run_test("Изменение свойств", test_properties)

    # Тест 3: Метод move()
    def test_move():
        p = TestParticle(5.0, 3.7, 2.5)
        p.move(1.0, 2.0)
        expected_x = 5.0 + 2.0 * 1.0
        assert p.x == expected_x, f"Ожидалось x={expected_x}, получено {p.x}"
    
    run_test("Тест метода move()", test_move)

    # Тест 4: Метод clone()
    def test_clone():
        p = TestParticle(7.0, 3.7, 2.5)
        p_clone = p.clone()
        assert isinstance(p_clone, TestParticle), "Клон не является экземпляром TestParticle"
        assert p_clone.x == p.x, f"Ожидалось x={p.x}, получено {p_clone.x}"
        assert p_clone.y == p.y, f"Ожидалось y={p.y}, получено {p_clone.y}"
    
    run_test("Тест метода clone()", test_clone)

    # Тест 5: Проверка __repr__
    def test_repr():
        p = TestParticle(2.5, 3.7, 1.0)
        expected_repr = f"Particle at ({p.x}, {p.y}) with size {p.size}"
        assert repr(p) == expected_repr, f"Ожидалось: '{expected_repr}', получено: '{repr(p)}'"
    
    run_test("Тест строкового представления", test_repr)

    print("\n=== Все тесты успешно пройдены! ===\n")

if __name__ == "__main__":
    try:
        main()
    except Exception:
        print("\n=== Тестирование завершено с ошибками ===")