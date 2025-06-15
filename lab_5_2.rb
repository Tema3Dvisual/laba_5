# Структура для хранения информации о посетителе
Visitor = Struct.new(:ticket, :time, :order)

# Функция сравнения для сортировки по времени (по убыванию)
compare_by_time_desc = lambda do |a, b|
  if a.time == b.time        # Если время одинаковое
    a.order < b.order        # Сортируем по порядковому номеру
  else
    a.time > b.time          # Иначе сортируем по убыванию времени
  end
end

# Функция сравнения для сортировки по порядковому номеру (по возрастанию)
compare_by_order = lambda { |a, b| a.order < b.order }

# Запрос количества окон
puts ">>> Введите кол-во окон"
windows_count = gets.chomp.to_i

visitors = []       # Массив для хранения посетителей
order_counter = 0   # Счетчик для генерации номеров билетов

# Основной цикл обработки команд
loop do
  command = gets.chomp
  next if command.empty?  # Пропуск пустых строк
  
  # Обработка команды ENQUEUE
  if command.start_with?("ENQUEUE")
    time = command.split[1].to_i
    
    # Генерация номера билета (формат T001, T002 и т.д.)
    ticket = "T%03d" % order_counter
    
    # Добавление посетителя в массив
    visitors << Visitor.new(ticket, time, order_counter)
    order_counter += 1
    
    # Вывод номера билета добавленного посетителя
    puts ">>> #{ticket}"
  # Обработка команды DISTRIBUTE (завершение ввода)
  elsif command == "DISTRIBUTE"
    break
  end
end

# Инициализация окон
windows = Array.new(windows_count) { [] }  # Массив окон с посетителями
window_time = Array.new(windows_count, 0)  # Время обработки для каждого окна

# Сортировка посетителей по времени (самые долгие сначала)
visitors.sort!(&compare_by_time_desc)

# Распределение посетителей по окнам
visitors.each do |visitor|
  # Поиск наименее загруженного окна
  min_index = 0
  min_time = window_time[0]
  
  (1...windows_count).each do |i|
    if window_time[i] < min_time
      min_time = window_time[i]
      min_index = i
    end
  end
  
  # Добавление посетителя в найденное окно
  windows[min_index] << visitor
  window_time[min_index] += visitor.time  # Увеличение времени обработки окна
end

# Сортировка посетителей в каждом окне по порядку прибытия
windows.each { |window| window.sort!(&compare_by_order) }

# Вывод результатов распределения
print ">>> "
windows.each_with_index do |window, i|
  # Вывод информации об окне
  print "Окно #{i+1} (#{window_time[i]} минут): "
  
  # Вывод билетов посетителей в окне
  puts window.map(&:ticket).join(', ')
  
  # Разделитель между окнами (кроме последнего)
  print " >>> " if i < windows_count - 1
end
puts
