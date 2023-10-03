path_flag=false

# Read parent pid, child pid
while [ "$#" -gt 0 ]; do
  case "$1" in
    --parent)
      parent="$2"
      shift 2
      ;;
    --child)
      child="$2"
      shift 2
      ;;
    --path)
      path_flag=true
      shift 1
      ;;
    *)
      echo "Unknown parameter passed: $1"
      exit 1
      ;;
  esac
done

# Check if parent or child is empty
if [ -z "$parent" ] || [ -z "$child" ]; then
  echo "Usage: $0 --parent PARENT_PID --child CHILD_PID [--path]"
  exit 1
fi

# The code below is only for demonstration purposes, please remove it before submitting.
# echo "parent pid: $parent, child pid: $child"

########
# TODO #
########

is_ancestor() {
  local child_pid=$1
  local parent_pid=$2
  local path=($child_pid)
  
  while [ $child_pid != 1 ]; do
    parent_of_child_pid=$(ps -o ppid= -p $child_pid)  # get parent PID

    path+=($parent_of_child_pid)  # trace the path

    if [ -z $parent_of_child_pid ]; then  # since parent is empty, it is impossible
      break
    fi

    if [ $parent_of_child_pid = $parent_pid ]; then # found ancestor relationship
      echo "Yes"

      if [ $path_flag = true ]; then  # print the path
        local n=${#path[@]}
        for ((i=n-1; i>=0; i--)) ; do
          if [ $i != $((n-1)) ]; then
            echo -n " -> "
          fi
          echo -n "${path[i]}"
        done
        echo ""
      fi
      return
    fi

    child_pid=$parent_of_child_pid  # look upper level

  done
  
  echo "No" # no ancestor relationship
}

is_ancestor $child $parent
