using System;
using System.Linq;
using System.Management.Automation;
using System.Reflection;

namespace RemotePowershellNet
{
    public static class PowershellExtensions
    {
        public static T Get<T>(this PSObject obj)
        {
            var targetType = typeof(T);
            var returnObj = (T)Activator.CreateInstance(targetType);
            var properties = targetType.GetProperties().Where(p => p.CanWrite && p.MemberType == MemberTypes.Property);

            foreach (PropertyInfo p in properties)
            {
                if (p.PropertyType == typeof(string))
                {
                    p.SetValue(returnObj, obj.Properties[p.Name].Value?.ToString());
                }
                else
                {
                    p.SetValue(returnObj, Convert.ChangeType(obj.Properties[p.Name].Value, p.PropertyType));
                }
            }

            return returnObj;
        }
    }
}
