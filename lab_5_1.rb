# Параметры склада
ZONES = ['A', 'B', 'C', 'D']
NUM_ZONES = 4
RACKS_PER_ZONE = 8
SECTIONS_PER_RACK = 2
SHELVES_PER_SECTION = 1
CELL_CAPACITY = 10
TOTAL_CAPACITY = NUM_ZONES * RACKS_PER_ZONE * SECTIONS_PER_RACK * SHELVES_PER_SECTION * CELL_CAPACITY # 640

# Структура для хранения данных ячейки
Cell = Struct.new(:products, :total) do
  def initialize
    self.products = Hash.new(0)
    self.total = 0
  end
end

# Глобальная переменная для склада
$warehouse = {}

# Инициализация склада
def initialize_warehouse
  ZONES.each do |zone|
    (1..RACKS_PER_ZONE).each do |rack|
      (1..SECTIONS_PER_RACK).each do |section|
        (1..SHELVES_PER_SECTION).each do |shelf|
          address = "#{zone}#{rack}#{section}#{shelf.to_s.rjust(2, '0')}"
          $warehouse[address] = Cell.new
        end
      end
    end
  end
end

# Проверка корректности адреса
def valid_address?(address)
  return false unless address.length == 5
  
  # Проверка зоны
  return false unless ZONES.include?(address[0])
  
  rack = address[1].to_i
  return false unless (1..RACKS_PER_ZONE).cover?(rack)
  
  section = address[2].to_i
  return false unless (1..SECTIONS_PER_RACK).cover?(section)
  
  shelf = address[3..4].to_i
  (1..SHELVES_PER_SECTION).cover?(shelf)
end

# Обработка команды ADD
def process_add(tokens)
  if tokens.size < 4
    puts "ERROR: Invalid ADD format. Use: ADD <product> <quantity> <address>"
    return
  end

  product = tokens[1]
  quantity = tokens[2].to_i
  
  if quantity <= 0
    puts "ERROR: Quantity must be positive"
    return
  end

  address = tokens[3]
  unless valid_address?(address)
    puts "ERROR: Invalid address. Format: ZRSSS (Z-zone, R-rack, S-section, SS-shelf)"
    return
  end

  cell = $warehouse[address]
  if cell.total + quantity > CELL_CAPACITY
    puts "ERROR: Exceeds cell capacity (max #{CELL_CAPACITY})"
    return
  end

  cell.products[product] += quantity
  cell.total += quantity
  puts "Added #{quantity} of #{product} to #{address}"
end

# Обработка команды REMOVE
def process_remove(tokens)
  if tokens.size < 4
    puts "ERROR: Invalid REMOVE format. Use: REMOVE <product> <quantity> <address>"
    return
  end

  product = tokens[1]
  quantity = tokens[2].to_i
  
  if quantity <= 0
    puts "ERROR: Quantity must be positive"
    return
  end

  address = tokens[3]
  unless valid_address?(address)
    puts "ERROR: Invalid address"
    return
  end

  cell = $warehouse[address]
  if cell.products[product].nil? || cell.products[product] < quantity
    puts "ERROR: Not enough products"
    return
  end

  cell.products[product] -= quantity
  cell.total -= quantity
  cell.products.delete(product) if cell.products[product] == 0
  puts "Removed #{quantity} of #{product} from #{address}"
end

# Обработка команды INFO
def process_info
  total_products = 0
  zone_products = Hash.new(0)
  product_counts = Hash.new(0)

  # Сбор статистики
  $warehouse.each do |address, cell|
    total_products += cell.total
    zone_products[address[0]] += cell.total
    
    cell.products.each do |product, qty|
      product_counts[product] += qty
    end
  end

  # Вывод общей информации
  puts "\n=== Warehouse Status ==="
  puts "Total capacity: #{TOTAL_CAPACITY}"
  puts "Current load: %.1f%% (%d items)" % [total_products.to_f / TOTAL_CAPACITY * 100, total_products]

  # Вывод по зонам
  puts "\nZone loads:"
  ZONES.each do |zone|
    load = zone_products[zone].to_f / 
           (RACKS_PER_ZONE * SECTIONS_PER_RACK * SHELVES_PER_SECTION * CELL_CAPACITY) * 100
    puts "Zone #{zone}: %.1f%% (%d items)" % [load, zone_products[zone]]
  end

  # Вывод статистики по продуктам
  unless product_counts.empty?
    puts "\nProducts in warehouse:"
    product_counts.each do |product, count|
      puts "#{product}: #{count}"
    end
  end

  # Вывод непустых ячеек
  non_empty_count = $warehouse.count { |_, cell| cell.total > 0 }
  puts "\nNon-empty cells: #{non_empty_count}/#{$warehouse.size}"

  # Вывод пустых ячеек (по запросу)
  puts "\nEnter 'SHOW EMPTY' to list empty cells or any command to continue..."
end

# Обработка команды SHOW EMPTY
def process_show_empty
  puts "Empty cells:"
  empty_cells = $warehouse.select { |_, cell| cell.total == 0 }.keys
  empty_cells.each_slice(10) { |slice| puts slice.join(' ') }
  puts "\nTotal empty: #{empty_cells.size}"
end

# Основная программа
initialize_warehouse

puts "=== Warehouse Management System ==="
puts "Available commands:"
puts "ADD <product> <quantity> <address>"
puts "REMOVE <product> <quantity> <address>"
puts "INFO - show warehouse status"
puts "SHOW EMPTY - list empty cells"
puts "QUIT - exit program"

loop do
  print "\n> "
  line = gets.chomp.strip
  next if line.empty?

  tokens = line.split
  command = tokens.first

  case command
  when "ADD"
    process_add(tokens)
  when "REMOVE"
    process_remove(tokens)
  when "INFO"
    process_info
  when "SHOW"
    process_show_empty if tokens.size > 1 && tokens[1] == "EMPTY"
  when "QUIT"
    puts "Exiting program..."
    break
  else
    puts "ERROR: Unknown command. Available commands: ADD, REMOVE, INFO, SHOW EMPTY, QUIT"
  end
end
