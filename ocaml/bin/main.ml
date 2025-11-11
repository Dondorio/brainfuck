open Bigarray
open Sexplib.Std
open Sexplib.Sexp

let explode_string s = List.init (String.length s) (String.get s)

type token = Plus | Minus | Left | Right | RepStart | RepEnd | Print | Input
[@@deriving sexp]

type ast = Num of int | Move of int | Rep of ast list | Print | Input
[@@deriving sexp]

let lex (input : string) =
  let f a =
    match a with
    | '+' -> Some Plus
    | '-' -> Some Minus
    | '<' -> Some Left
    | '>' -> Some Right
    | '[' -> Some RepStart
    | ']' -> Some RepEnd
    | '.' -> Some Print
    | ',' -> Some Input
    | _ -> None
  in
  List.filter_map f (explode_string input)

let parse (input : token list) =
  let rec collapse acc t input =
    let f x =
      match t with
      | `Num -> (
          match x with Plus -> Some 1 | Minus -> Some (-1) | _ -> None)
      | `Move -> (
          match x with Right -> Some 1 | Left -> Some (-1) | _ -> None)
    in
    match input with
    | [] -> (acc, input)
    | a :: rest -> (
        match f a with
        | Some count -> collapse (acc + count) t rest
        | None -> (acc, input))
  in

  let rec f acc (r : token list) =
    match r with
    | [] -> (acc, r)
    | Print :: rest -> f (Print :: acc) rest
    | Input :: rest -> f (Input :: acc) rest
    | RepEnd :: rest -> (acc, rest)
    | RepStart :: rest ->
        let inner, new_rest = f [] rest in
        f (Rep (List.rev inner) :: acc) new_rest
    | Plus :: _ | Minus :: _ ->
        let count, rest = collapse 0 `Num r in
        f (Num count :: acc) rest
    | Left :: _ | Right :: _ ->
        let count, rest = collapse 0 `Move r in
        f (Move count :: acc) rest
  in
  match f [] input with
  | a, [] -> List.rev a
  | _, _ -> failwith "Unclosed delimiter"

let size = 30000
let data = Array1.create int8_unsigned c_layout size;;

Array1.fill data 0;;

let pointer = ref 0
let set_current = fun d -> Array1.set data !pointer d
let get_current = fun () -> Array1.get data !pointer

let modulo x y =
  let result = x mod y in
  if result >= 0 then result else result + y

let rec eval = function
  | first :: rest ->
      (match first with
      | Num count -> set_current (get_current () + count)
      | Move count -> pointer := modulo (!pointer + count) size
      | Print ->
          print_char (Array1.get data !pointer |> char_of_int);
          flush stdout
      | Input ->
          (* TODO clear stdin after getting char input *)
          set_current (input_char stdin |> int_of_char)
      | Rep inner ->
          while get_current () != 0 do
            eval inner
          done);
      eval rest
  | [] -> ()

let tok = lex (read_line ())
let ast = parse tok;;

Printf.printf "ast: %s\n" (sexp_of_list sexp_of_ast ast |> to_string_hum);
flush stdout;

eval ast
;;

for i = 0 to 10 do
  print_int (Array1.get data i);
  print_char ' '
done
