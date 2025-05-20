import pygame
import sys
import random
import math
import subprocess
import particle_sim as ps

# Конфигурация интерфейса
WINDOW_WIDTH = 1200
WINDOW_HEIGHT = 750
SIM_AREA_WIDTH = 800
SIM_AREA_HEIGHT = 500
BUTTON_WIDTH = 200
BUTTON_HEIGHT = 40
INPUT_BOX_WIDTH = 200
INPUT_BOX_HEIGHT = 32
SIM_AREA_X = 250
SIM_AREA_Y = 50
INFO_PANEL_X = 20
INFO_PANEL_Y = 350

COLORS = {
    "bacteria": (0, 255, 0),
    "nutrition": (0, 0, 255),
    "matrix": (128, 128, 128),
    "background": (30, 30, 30),
    "frame": (200, 200, 200),
    "button": (70, 70, 70),
    "button_hover": (100, 100, 100),
    "text": (255, 255, 255),
    "speed_indicator": (200, 150, 50),
    "search_result": (255, 0, 0)
}

class Button:
    def __init__(self, x, y, text, action, width=BUTTON_WIDTH, height=BUTTON_HEIGHT):
        self.rect = pygame.Rect(x, y, width, height)
        self.text = text
        self.action = action
        self.hover = False

    def draw(self, screen):
        color = COLORS["button_hover"] if self.hover else COLORS["button"]
        pygame.draw.rect(screen, color, self.rect)
        font = pygame.font.Font(None, 24)
        text_surf = font.render(self.text, True, COLORS["text"])
        text_rect = text_surf.get_rect(center=self.rect.center)
        screen.blit(text_surf, text_rect)




class Simulation:
    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
        self.font = pygame.font.SysFont("Arial", 24)
        self.info_font = pygame.font.SysFont("Arial", 22)
        self.clock = pygame.time.Clock()
        
        # Состояние симуляции
        self.simulation_running = False
        self.time_scale = 1.0
        self.min_time_scale = 0.1
        self.max_time_scale = 5.0
        self.info_text = {
            "bacteria": "",
            "nutrition": "",
            "matrix": ""
        }

        # Элементы управления
        self.input_box = pygame.Rect(20, 600, INPUT_BOX_WIDTH, INPUT_BOX_HEIGHT)
        self.input_text = ""
        self.search_results = []
        self.search_logs = []
        self.tree = None
        
        # Инициализация компонентов
        self.init_components()
        self.input_active = False  # Состояние активности поля ввода

    def init_components(self):
        self.sim_area = pygame.Rect(
            SIM_AREA_X,
            SIM_AREA_Y,
            SIM_AREA_WIDTH,
            SIM_AREA_HEIGHT
        )
        
        self.buttons = [
            Button(20, 50, "Старт/Стоп", self.toggle_simulation),
            Button(20, 120, "Показать бактерии", self.show_bacteria_info),
            Button(20, 190, "Поиск в области", self.search_area),
            Button(20, 260, "Построить KD-дерево", self.build_kdtree),
            Button(20, 400, "-", self.slow_down, 50, 40),
            Button(80, 400, "+", self.speed_up, 50, 40)
        ]
        
        self.particles = []
        self.map = ps.Map(SIM_AREA_WIDTH, SIM_AREA_HEIGHT)
        self.init_particles()

#генерим частицы
    def init_particles(self):
    # Бактерии
        for _ in range(5):
            b = ps.Bacteria(
                random.uniform(10, SIM_AREA_WIDTH-10),
                random.uniform(10, SIM_AREA_HEIGHT-10),
                10.0,
                random.uniform(0, 2*math.pi),
                1.0,
                50.0
            )
            self.particles.append(b)
            self.map.add_object(b)

        # Питательные частицы
        for _ in range(100):
            n = ps.NutritionParticle(
                random.uniform(10, SIM_AREA_WIDTH-10),
                random.uniform(10, SIM_AREA_HEIGHT-10),
                5.0,
                2.0
            )
            self.particles.append(n)
            self.map.add_object(n)
        
        # Матричные частицы
        for _ in range(5):
            m = ps.MatrixParticle(
                random.uniform(10, SIM_AREA_WIDTH-10),
                random.uniform(10, SIM_AREA_HEIGHT-10),
                8.0
            )
            self.particles.append(m)
            self.map.add_object(m)





#Структура

#кнопка Построить кд-дерево. Строит кд-дерево из текущих координат частиц
    def build_kdtree(self):
        if not self.particles:
            print("Нет частиц для построения дерева!")
            return
        points = [ps.Point(int(p.x), int(p.y)) for p in self.particles] #список частиц
        self.tree = ps.KdTree.build(points, ps.Axis.X) 
        print(f"Построено дерево с {len(points)} узлами")  

#кнопка поиск в области. ищет частицы в прямоугольной области на основе введеных координат Х, У  
    def search_area(self):
        if not self.tree:
            self.search_logs.append("Сначала постройте KD-дерево!")
            return

        # Проверка пустого ввода
        input_text = self.input_text.strip().replace(" ", "")
        if not input_text:
            self.search_logs.append("Введите координаты (например: '400,300')")
            return

        try:
            # Проверка формата и преобразование координат
            if input_text.count(",") != 1:
                raise ValueError("Некорректный формат. Используйте X,Y")

            cx, cy = map(int, input_text.split(","))
            
            # Преобразование экранных координат в координаты симуляции
            sim_cx = cx - SIM_AREA_X
            sim_cy = cy - SIM_AREA_Y

            # Поиск в уменьшенной области для точности
            lower = ps.Point(sim_cx - 100, sim_cy - 100)
            upper = ps.Point(sim_cx + 100, sim_cy + 100)
            
            writer = ps.KdTree.search_range(lower, upper, self.tree) #на выходе получили векторс наййденнымиточкаи
            self.search_results = writer.get_value() #извлекли значения
            self.search_logs.append(f"Найдено частиц: {len(self.search_results)}")

        except ValueError as e:
            self.search_logs.append(f"Ошибка: {str(e)}")
        except Exception as e:
            self.search_logs.append(f"Системная ошибка: {str(e)}")
        finally:
            self.input_text = ""

#Обрабатывает события ввода (клики мыши и нажатия клавиш),
#  включая активацию кнопок и ввод координат для поиска.    
    def handle_input(self, event):
        """Обработка событий ввода"""
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:  # Левая кнопка мыши
                # Обработка клика по кнопкам
                for btn in self.buttons:
                    if btn.rect.collidepoint(event.pos):
                        btn.action()
                
                # Переключение активности поля ввода
                self.input_active = self.input_box.collidepoint(event.pos)
                
                # Отладочный вывод
                print(f"Клик на координатах: {event.pos}")
                print(f"Поле ввода активно: {self.input_active}")

        elif event.type == pygame.KEYDOWN and self.input_active:
            print(f"Обработка клавиши: {event.unicode}")  # Отладочный вывод
            if event.key == pygame.K_RETURN:
                self.process_input()
            elif event.key == pygame.K_BACKSPACE:
                self.input_text = self.input_text[:-1]
            else:
                # Ограничение на ввод только цифр и запятой
                if event.unicode in "0123456789,":
                    self.input_text += event.unicode

#Обрабатывает введенные координаты, запуская поиск в области, если ввод не пустой
    def process_input(self):
        if self.input_text.strip():
            self.search_area()  # Автоматический поиск при нажатии Enter
        self.input_text = ""

#отрисовка поле ввода и последние три записи лога поиска
    def draw_interface(self):
        """Отрисовка элементов интерфейса"""
        # Поле ввода с изменяемым фоном
        bg_color = (255, 255, 255) if self.input_active else COLORS["background"]
        pygame.draw.rect(self.screen, bg_color, self.input_box)  # Фон
        pygame.draw.rect(self.screen, COLORS["text"], self.input_box, 2)  # Рамка
        
        # Текст с черным цветом для контраста
        txt_surface = self.font.render(self.input_text, True, (0, 0, 0))
        self.screen.blit(txt_surface, (self.input_box.x + 5, self.input_box.y + 5))
        
        # Логи поиска (последние 3 записи)
        log_y = 550
        for log in self.search_logs[-3:]:
            log_surf = self.font.render(log, True, COLORS["text"])
            self.screen.blit(log_surf, (20, log_y))
            log_y += 20

#Отрисовывает результаты поиска (точки, найденные в KD-дереве)
#  в виде красных окружностей в области симуляции
    def draw_search_results(self):
        """Отрисовка результатов поиска в области симуляции"""
        if not self.search_results:
            return
            
        for point in self.search_results:
            # Преобразуем координаты из системы частиц в экранные
            x = int(point.x) + SIM_AREA_X
            y = int(point.y) + SIM_AREA_Y
            
            # Рисуем красный кружок с обводкой
            pygame.draw.circle(
                self.screen, 
                COLORS["search_result"], 
                (x, y), 
                8,  # Радиус
                2   # Толщина обводки
            )





#Prolog

# сохраняет текущие данные о частицах (их тип и координаты) 
# в файл current_particles.pl в формате (particle(bacteria, 123.45, 678.90).)
    def save_particles_to_prolog(self):
        with open("current_particles.pl", "w") as f:
            for p in self.particles:

                if isinstance(p, ps.Bacteria):
                    type_ = "bacteria"
                elif isinstance(p, ps.NutritionParticle):
                    type_ = "nutrition"
                elif isinstance(p, ps.MatrixParticle):
                    type_ = "matrix"
                else:
                    continue
                f.write(f"particle({type_}, {p.x:.2f}, {p.y:.2f}).\n")


# выполняет запрос к Prolog для поиска частиц заданного типа (particle_type) 
# и возвращает их координаты, полученные из вывода Prolog.
    def query_prolog(self, particle_type):
        self.save_particles_to_prolog()  # Обновляем данные
        
        # Запуск Prolog-запроса
        result = subprocess.run(
            [
                "swipl",                #запускает интерпретатор SWI-Prolog
                "-q",                   # Тихий режим
                "-g", f"find_particles_by_type({particle_type}, Positions), writeln(Positions).",
                "-t", "halt",           #выполняет запрос find_particles_by_type для заданного типа ) 
                                        #и выводит результат в переменную Positions
                "particle_rules.pl",    # правила
                "current_particles.pl"  # Текущие данные
            ],
            capture_output=True,
            text=True
        )
        
        # Пример вывода: [(100.5,200.3),(50.1,70.9)]
        output = result.stdout.strip().replace(" ", "") #strip() удаляет лишние пробелы и символы 
                                                        #новой строки в начале и конце; replace(" ", "") 
                                                        #удаляет все пробелы для упрощения парсинга
        
        return self.parse_prolog_output(output) #возвращает список кортежей с координатами частиц
                                                #заданного типа, полученный после парсинга вывода Prolog


#парсит строковый вывод Prolog (список координат в формате [(X1,Y1),(X2,Y2),...])
# и преобразует его в список кортежей Python [(x1, y1), (x2, y2), ...].   
    def parse_prolog_output(self, output):
        if not output or output == "[]":   #Проверка пустого вывода
            return []
        
        try:
            # Удаление лишних символов и разбиение на элементы
            cleaned = output.strip().replace(" ", "").replace("[", "").replace("]", "")
            if not cleaned:  #[(100.5,200.3),(50.1,70.9)] получается (100.5,200.3),(50.1,70.9)
                return []
            
            items = cleaned.split("),(")  #Разбиение на элементы
            positions = []                #"(100.5,200.3", "50.1,70.9)"
            
            for item in items:
                # Обработка крайних случаев с скобками
                item = item.replace("(", "").replace(")", "")
                if not item or ',' not in item:
                    continue
                    
                coords = item.split(',', 1)  # Разбиваем только по первой запятой для ["100.5", "200.3"]
                if len(coords) != 2:
                    continue
                    
                try:
                    x = float(coords[0])  #строки преобразуются в числа с плавающей точкой
                    y = float(coords[1])   
                    positions.append((x, y))
                except ValueError:
                    continue
            
            return positions
        except Exception as e:
            print(f"Ошибка парсинга вывода Prolog: {e}")
            return []


#В словарь self.info_text записывается строка с информацией о бактериях
#  ("Бактерии (5):\n1. X: 100.5 Y: 200.3\n..."), а информация
#  о других типах частиц сбрасывается.     
    def show_bacteria_info(self):
        self.info_text["bacteria"] = self.get_particles_info(ps.Bacteria, "Бактерии")
        self.info_text["nutrition"] = ""
        self.info_text["matrix"] = ""

    def show_nutrition_info(self):
        self.info_text["nutrition"] = self.get_particles_info(ps.NutritionParticle, "Питательные")
        self.info_text["bacteria"] = ""
        self.info_text["matrix"] = ""

    def show_matrix_info(self):
        self.info_text["matrix"] = self.get_particles_info(ps.MatrixParticle, "Матрикс")
        self.info_text["bacteria"] = ""
        self.info_text["nutrition"] = ""


#Этот метод формирует текстовую строку с информацией о частицах заданного типа,
# используя данные из Prolog
    def get_particles_info(self, particle_type, title):
        # Определение типа для Prolog
        type_map = {               #словарь, связывающий классы Python/C++
                                   # с их строковыми эквивалентами в Prolog
            ps.Bacteria: "bacteria",
            ps.NutritionParticle: "nutrition",
            ps.MatrixParticle: "matrix"
        }
        prolog_type = type_map.get(particle_type, "")
        
        # Получение данных из Prolog
        positions = self.query_prolog(prolog_type)
        
        # Формирование текста
        info = [f"{title} ({len(positions)}):"] 
        for i, (x, y) in enumerate(positions[:5]): 
            info.append(f"{i+1}. X: {x:.1f} Y: {y:.1f}")
        if len(positions) > 5:
            info.append("...")
        return "\n".join(info)    







# старт\стоп симуляции - прямое отношение к кнопке
    def toggle_simulation(self):
        self.simulation_running = not self.simulation_running

#увеличивает масштаб времени симуляции +
    def speed_up(self):
        self.time_scale = min(self.max_time_scale, self.time_scale * 1.5)

#уменьшает масштаб времени симуляции - 
    def slow_down(self):
        self.time_scale = max(self.min_time_scale, self.time_scale / 1.5)    




#Физика частиц

#обработка взаимодействий
    def check_collisions(self):
        to_remove = []
        to_add = []

        for i in range(len(self.particles)):
            p1 = self.particles[i]
            if p1 is None:
                continue

            # Проверка на критический размер бактерии
            if isinstance(p1, ps.Bacteria) and p1.size >= 20:
                to_add.append(ps.MatrixParticle(p1.x, p1.y, 8.0))
                p1.x = random.uniform(p1.size, SIM_AREA_WIDTH - p1.size)
                p1.y = random.uniform(p1.size, SIM_AREA_HEIGHT - p1.size)
                p1.size = 10.0

            for j in range(i + 1, len(self.particles)):
                p2 = self.particles[j]
                if p2 is None:
                    continue

                dx = p1.x - p2.x
                dy = p1.y - p2.y
                distance = math.hypot(dx, dy)
                min_distance = p1.size + p2.size

                if distance < min_distance:
                    # Бактерия и питательная частица
                    if isinstance(p1, ps.Bacteria) and isinstance(p2, ps.NutritionParticle):
                        p1.size += p2.nutrient_value
                        to_remove.append(p2)
                    elif isinstance(p2, ps.Bacteria) and isinstance(p1, ps.NutritionParticle):
                        p2.size += p1.nutrient_value
                        to_remove.append(p1)

                    # Отталкивание для других типов
                    else:
                        angle = math.atan2(dy, dx)
                        overlap = min_distance - distance
                        if not isinstance(p1, ps.MatrixParticle):
                            p1.x += math.cos(angle) * overlap / 2
                            p1.y += math.sin(angle) * overlap / 2
                        if not isinstance(p2, ps.MatrixParticle):
                            p2.x -= math.cos(angle) * overlap / 2
                            p2.y -= math.sin(angle) * overlap / 2

        # Безопасное удаление частиц
        for p in to_remove:
            try:
                if p in self.particles:
                    self.particles.remove(p)
                    if hasattr(self.map, 'remove_object'):
                        self.map.remove_object(p)
            except Exception as e:
                print(f"Ошибка при удалении частицы: {e}")

        # Добавление новых частиц матрикса
        for p in to_add:
            self.particles.append(p)
            self.map.add_object(p)

#чтобы частицы не заходили на границы
    def handle_boundaries(self, particle):
        size = particle.size
        particle.x = max(size, min(particle.x, SIM_AREA_WIDTH - size))
        particle.y = max(size, min(particle.y, SIM_AREA_HEIGHT - size))

#рандомное движение частиц
    def update_movement(self):
        if not self.simulation_running:
            return

        # Обновляем движение для всех частиц
        for p in self.particles:
            if isinstance(p, ps.Bacteria):
                # Настройки для бактерий
                angle = random.uniform(0, 2 * math.pi)
                speed = max(0.5, 500.0 / p.size) * self.time_scale  # Регулировка скорости
                p.move(angle, speed)
                self.handle_boundaries(p)

            elif isinstance(p, ps.NutritionParticle):
                # Настройки для питательных частиц
                angle = random.uniform(0, 2 * math.pi)
                speed = max(0.5, 5.0) * self.time_scale  # Нормальная скорость
                p.move(angle, speed)
                self.handle_boundaries(p)

        # Проверка коллизий после обновления всех позиций
        self.check_collisions()
        


    
#Отрисовка
    def draw_info_panel(self):
        y = INFO_PANEL_Y
        for key in ["bacteria", "nutrition", "matrix"]:
            if self.info_text[key]:
                lines = self.info_text[key].split('\n')
                for line in lines:
                    text_surf = self.info_font.render(line, True, COLORS["text"])
                    self.screen.blit(text_surf, (INFO_PANEL_X, y))
                    y += 20
                y += 10
  
    def draw_frame(self):
        """Отрисовывает рамку области симуляции"""
        pygame.draw.rect(
            self.screen, 
            COLORS["frame"], 
            self.sim_area, 
            2
        )
        text = self.font.render("Simulation Area", True, COLORS["frame"])
        text_rect = text.get_rect(
            center=(self.sim_area.centerx, self.sim_area.top - 15)
        )
        self.screen.blit(text, text_rect)

    def draw_speed_indicator(self):
        # Отрисовка индикатора скорости
        text = f"Скорость: x{self.time_scale:.1f}"
        text_surf = self.font.render(text, True, COLORS["speed_indicator"])
        text_rect = text_surf.get_rect(topleft=(20, 460))
        self.screen.blit(text_surf, text_rect) 

    def draw_particles(self):
        """Отрисовка всех частиц"""
        for p in self.particles:
            if p is None:
                continue
            color = COLORS["bacteria"] if isinstance(p, ps.Bacteria) else \
                    COLORS["nutrition"] if isinstance(p, ps.NutritionParticle) else \
                    COLORS["matrix"]
            pos = (int(p.x + self.sim_area.x), int(p.y + self.sim_area.y))
            pygame.draw.circle(self.screen, color, pos, int(p.size))

    
    
    def run(self):
        running = True
        while running:
            mouse_pos = pygame.mouse.get_pos()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                self.handle_input(event)

            # Обновление физики
            if self.simulation_running:
                self.update_movement()

            # Отрисовка
            self.screen.fill(COLORS["background"])
            
            # Область симуляции
            self.draw_frame()
            self.draw_particles()
            
            # Интерфейс
            self.draw_search_results()
            self.draw_interface()
            self.draw_info_panel()
            self.draw_speed_indicator()
            
            # Кнопки
            for btn in self.buttons:
                btn.hover = btn.rect.collidepoint(mouse_pos)
                btn.draw(self.screen)

            pygame.display.flip()
            self.clock.tick(60)

        pygame.quit()

if __name__ == "__main__":
    sim = Simulation()
    sim.run()